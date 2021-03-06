/**
 * StructureEventsWriter.cpp
 *
 * Copyright (C) 2009-2015 by MegaMol Team
 * Copyright (C) 2015 by Richard H�hne, TU Dresden
 * Alle Rechte vorbehalten.
 */

#include "stdafx.h"
#include "StructureEventsWriter.h"

#include "mmcore/param/FilePathParam.h"
#include "vislib/sys/Log.h"
#include "vislib/sys/FastFile.h"
#include "vislib/String.h"
#include "vislib/sys/Thread.h"

using namespace megamol;
using namespace megamol::core;

/**
 * mmvis_static::StructureEventsWriter::StructureEventsWriter
 */
mmvis_static::StructureEventsWriter::StructureEventsWriter() : AbstractDataWriter(),
	filenameSlot("filename", "The path to the MMSE file to be written"),
	dataSlot("data", "The slot requesting the data to be written") {

	this->filenameSlot.SetParameter(new param::FilePathParam("eventsFromMPDC.mmse"));
	this->MakeSlotAvailable(&this->filenameSlot);

	this->dataSlot.SetCompatibleCall<StructureEventsDataCallDescription>();
	this->MakeSlotAvailable(&this->dataSlot);
}


/**
 * mmvis_static::StructureEventsWriter::~StructureEventsWriter
 */
mmvis_static::StructureEventsWriter::~StructureEventsWriter(void) {
	this->Release();
}


/**
 * mmvis_static::StructureEventsWriter::create
 */
bool mmvis_static::StructureEventsWriter::create(void) {
	// intentionally empty
	return true;
}


/**
 * mmvis_static::StructureEventsWriter::release
 */
void mmvis_static::StructureEventsWriter::release(void) {
}


/**
 * mmvis_static::StructureEventsWriter::run
 */
bool mmvis_static::StructureEventsWriter::run(void) {
	using vislib::sys::Log;

	///
	/// Check slots availability.
	///
	vislib::TString filename(this->filenameSlot.Param<param::FilePathParam>()->Value());
	if (filename.IsEmpty()) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR,
			"No file name specified. Abort.");
		return false;
	}

	StructureEventsDataCall *sedc = this->dataSlot.CallAs<StructureEventsDataCall>();
	if (sedc == NULL) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR,
			"No data source connected. Abort.");
		return false;
	}

	if (vislib::sys::File::Exists(filename)) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_WARN,
			"File %s already exists and will be overwritten.",
			vislib::StringA(filename).PeekBuffer());
	}

	vislib::math::Cuboid<float> bbox;
	vislib::math::Cuboid<float> cbox;

	///
	/// Data extends.
	///
	if (!(*sedc)(1)) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_WARN, "Unable to query data extents.");
		bbox.Set(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		cbox.Set(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
	}
	// Set bounding boxes.
	else {
		if (sedc->AccessBoundingBoxes().IsObjectSpaceBBoxValid()
			|| sedc->AccessBoundingBoxes().IsObjectSpaceClipBoxValid()) {
			if (sedc->AccessBoundingBoxes().IsObjectSpaceBBoxValid()) {
				bbox = sedc->AccessBoundingBoxes().ObjectSpaceBBox();
			}
			else {
				bbox = sedc->AccessBoundingBoxes().ObjectSpaceClipBox();
			}
			if (sedc->AccessBoundingBoxes().IsObjectSpaceClipBoxValid()) {
				cbox = sedc->AccessBoundingBoxes().ObjectSpaceClipBox();
			}
			else {
				cbox = sedc->AccessBoundingBoxes().ObjectSpaceBBox();
			}
		}
		else {
			Log::DefaultLog.WriteMsg(Log::LEVEL_WARN, "Object space bounding boxes not valid. Using defaults");
			bbox.Set(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
			cbox.Set(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	///
	/// Create file.
	///
	vislib::sys::FastFile file;
	if (!file.Open(filename, vislib::sys::File::WRITE_ONLY, vislib::sys::File::SHARE_EXCLUSIVE, vislib::sys::File::CREATE_OVERWRITE)) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR,
			"Unable to create output file \"%s\". Abort.",
			vislib::StringA(filename).PeekBuffer());
		sedc->Unlock();
		return false;
	}

#define ASSERT_WRITEOUT(A, S) if (file.Write((A), (S)) != (S)) { \
        Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR, "Write error %d", __LINE__); \
        file.Close(); \
        sedc->Unlock(); \
        return false; \
	    }

	///
	/// Set header.
	///
	vislib::StringA magicID("MMSE");
	ASSERT_WRITEOUT(magicID.PeekBuffer(), 4); // Only works with 4.
	//UINT16 version = 0; // Um Zustand zu dokumentieren, bis wohin geschrieben wurde? Ein bisschen und "The version number encodes the major version as multiple of 100 and adds the minor version."
	//ASSERT_WRITEOUT(&version, 2);
	//ASSERT_WRITEOUT(&frameCnt, 4);
	ASSERT_WRITEOUT(bbox.PeekBounds(), 6 * 4); // 6 * float.
	ASSERT_WRITEOUT(cbox.PeekBounds(), 6 * 4); // 6 * float.

	//UINT64 seekTable = static_cast<UINT64>(file.Tell()); // Position of current file pointer: To be able to set offset.
	sedc->Unlock();
	//file.Seek(seekTable); // Move the file pointer. Not needed here as well as file.Tell().

		Log::DefaultLog.WriteMsg(Log::LEVEL_INFO, "Started writing data.\n");
		/* Get frame or something.
		int missCnt = -9;
		do {
			sedc->Unlock();
			sedc->SetFrameID(i, true);
			if (!(*sedc)(0)) {
				Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR, "Cannot get data frame %u. Abort.\n", i);
				file.Close();
				return false;
			}
			if (sedc->FrameID() != i) {
				if ((missCnt % 10) == 0) {
					Log::DefaultLog.WriteMsg(Log::LEVEL_WARN,
						"Frame %u returned on request for frame %u\n", sedc->FrameID(), i);
				}
				++missCnt;
				vislib::sys::Thread::Sleep(static_cast<DWORD>(1 + std::max(missCnt, 0) * 100));
			}
		} while (sedc->FrameID() != i);
		*/
	if (!(*sedc)(0)) {
		Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR, "Cannot get structure events data. Abort.\n");
		file.Close();
		return false;
	}

	if (!this->writeData(file, *sedc)) {
		sedc->Unlock();
		Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR, "Cannot write data. Abort.\n");
		file.Close();
		return false;
	}
	sedc->Unlock();

	//file.Seek(6); // set correct version to show that file is complete, 6 = reserved for ID
	//version = 100;
	//ASSERT_WRITEOUT(&version, 2);

	//file.Seek(frameOffset); // wozu? vielleicht, dass ein weiterer Prozess noch Daten anh�ngen kann?

	Log::DefaultLog.WriteMsg(Log::LEVEL_INFO, "Completed writing data\n");
	file.Close();

#undef ASSERT_WRITEOUT
	return true;
}


bool mmvis_static::StructureEventsWriter::writeData(vislib::sys::File& file, StructureEventsDataCall& data) {
	using vislib::sys::Log;

#define ASSERT_WRITEOUT(A, S) if (file.Write((A), (S)) != (S)) { \
        Log::DefaultLog.WriteMsg(Log::LEVEL_ERROR, "Write error %d", __LINE__); \
        file.Close(); \
        return false; \
    }

	StructureEvents events = data.getEvents();
	
	///
	/// Header continued.
	///
	uint64_t eventCnt = events.getCount();
	ASSERT_WRITEOUT(&eventCnt, 8);
	float maxTime = events.getMaxTime();
	ASSERT_WRITEOUT(&maxTime, 4);

	///
	/// Daten.
	///
	unsigned int eventStride = events.getStride();
	// Start of event data. Cast to 1 byte for correct stride calculation in for-loop. Mandatory!
	const unsigned char* eventPtr = static_cast<const unsigned char*>(events.getLocation());

	// Write each event and not all at once since file.Write() needs to know the buffer size.
	// Alternative?
	//const void *pointer = events.getLocation(); // Start of the eventdata.
	//ASSERT_WRITEOUT(pointer, eventStride * eventCnt);
	for (uint64_t eventIndex = 0; eventIndex < eventCnt; ++eventIndex) {
		// Alle Properties (stride).Typ des Pointers egal, da file.Write -> void*
		ASSERT_WRITEOUT(eventPtr, eventStride);
		eventPtr += eventStride; // Increment the position of the Pointer.
	}

	Log::DefaultLog.WriteMsg(Log::LEVEL_INFO, "%d events written.", eventCnt);

#undef ASSERT_WRITEOUT
	return true;
}


/**
 * mmvis_static::StructureEventsWriter::getCapabilities
 */
bool mmvis_static::StructureEventsWriter::getCapabilities(DataWriterCtrlCall& call) {
	call.SetAbortable(false);
	return true;
}