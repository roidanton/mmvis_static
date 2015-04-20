/**
 * StructureEventsWriter.h
 *
 * Copyright (C) 2009-2015 by MegaMol Team
 * Alle Rechte vorbehalten.
 */

#ifndef MMVISSTATIC_StructureEventsWriter_H_INCLUDED
#define MMVISSTATIC_StructureEventsWriter_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "mmcore/AbstractDataWriter.h"
#include "mmcore/CallerSlot.h"
#include "mmcore/param/ParamSlot.h"

namespace megamol {
	namespace mmvis_static {
		/**
		 * TODO: This class is a stub!
		 core::utility::xml::XMLReader
		 #include "mmcore/utility/xml/XmlReader.h"
		 */
		class StructureEventsWriter : public core::AbstractDataWriter {
		public:
			/**
			 * Answer the name of this module.
			 *
			 * @return The name of this module.
			 */
			static const char *ClassName(void) {
				return "StructureEventsWriter";
			}

			/**
			 * Answer a human readable description of this module.
			 *
			 * @return A human readable description of this module.
			 */
			static const char *Description(void) {
				return "MMSE file writer.";
			}

			/**
			 * Answers whether this module is available on the current system.
			 *
			 * @return 'true' if the module is available, 'false' otherwise.
			 */
			static bool IsAvailable(void) {
				return true;
			}

			/**
			* Disallow usage in quickstarts
			*
			* @return false
			*/
			static bool SupportQuickstart(void) {
				return false;
			}

			/** Ctor. */
			StructureEventsWriter(void);

			/** Dtor. */
			virtual ~StructureEventsWriter(void);

		protected:

			/**
			* Implementation of 'Create'.
			*
			* @return 'true' on success, 'false' otherwise.
			*/
			virtual bool create(void);

			/**
			* Implementation of 'Release'.
			*/
			virtual void release(void);

			/**
			* The main function
			*
			* @return True on success
			*/
			virtual bool run(void);

			/**
			* Function querying the writers capabilities
			*
			* @param call The call to receive the capabilities
			*
			* @return True on success
			*/
			virtual bool getCapabilities(core::DataWriterCtrlCall& call);

		private:

			/** The file name */
			core::param::ParamSlot filenameSlot;

			/** The slot asking for data */
			core::CallerSlot dataSlot;
		};

	} /* namespace mmvis_static */
} /* namespace megamol */

#endif /* MMVISSTATIC_StructureEventsWriter_H_INCLUDED */