/**
 * StructureEventsDataCall.h
 *
 * Copyright (C) 2009-2015 by MegaMol Team
 * Copyright (C) 2015 by Richard H�hne, TU Dresden
 * Alle Rechte vorbehalten.
 *
 * Uses pointer for data instead of copying into own
 * structure. Either use:
 * - one pointer for all datatypes and all events
 * - one pointer for all datatypes and each event
 * - one pointer for each datatype and all events
 * - one pointer for each datatype and each event
 * Not yet decided: Depends on Calculation output.
 */

#ifndef MMVISSTATIC_StructureEventsDataCall_H_INCLUDED
#define MMVISSTATIC_StructureEventsDataCall_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "mmcore/AbstractGetData3DCall.h"
#include "mmcore/factories/CallAutoDescription.h"
#include "glm/glm/glm.hpp"
#include <vector>

namespace megamol {
	namespace mmvis_static {

		/**
		 * Container for all events.
		 * The event attributes are the position, the time, the type
		 * as well as an agglomeration matrix.
		 *
		 * @remark Future optimizations: Type doesn't need to be stored
		 * per event but instead four lists of events could be used,
		 * one for each event type.
		 */
		class StructureEvents {
		public:

			/** Possible values for the event type */
			enum EventType {
				BIRTH,
				DEATH,
				MERGE,
				SPLIT
			};

			/// Ctor.
			StructureEvents(void);

			/// Dtor.
			virtual ~StructureEvents(void);

			/// Ctor.
			StructureEvents(const StructureEvents& src);

			/**
			 * @return The location pointer
			 */
			inline const float * getLocation(void) const {
				return this->locationPtr;
			}

			/**
			 * @return The time pointer
			 */
			inline const float * getTime(void) const {
				return this->timePtr;
			}

			/**
			 * @return The type pointer
			 */
			inline const uint8_t * getType(void) const {
				return this->typePtr;
			}

			/**
			 * @return The stride
			 */
			inline unsigned int getStride(void) const {
				return this->stride;
			}

			/**
			 * @return The calculated stride.
			 */
			inline unsigned int getCalculatedStride(void) const {
				return 4 * sizeof(float) + sizeof(uint8_t);
			}

			inline void setStride(unsigned int stride) {
				printf("Data call set stride %d.\n\n", stride); // Debug.
				this->stride = stride;
			}

			inline void setEvents(
				const float *location,
				const float *time,
				const uint8_t *type,
				unsigned int stride,
				uint64_t count) {
				this->locationPtr = location;
				this->timePtr = time;
				this->typePtr = type;
				this->stride = stride;
				this->count = count;
			}

			/**
			 * Answer the event type.
			 *
			 * @return The event type as EventType.
			 */
			inline EventType getEventType(uint8_t typeCode) const {
				switch (typeCode){
				case 0:
					return this->BIRTH;
				case 1:
					return this->DEATH;
				case 2:
					return this->MERGE;
				case 3:
					return this->SPLIT;
				}
			};

			inline uint64_t getCount(void) const {
				return this->count;
			};

			inline float getMaxTime() const {
				return maxTime;
			};

			/**
			 * Assignment operator.
			 * Makes a deep copy of all members. While for data these are only
			 * pointers, the pointer to the unlocker object is also copied.
			 *
			 * @param rhs The right hand side operand
			 *
			 * @return A reference to this
			 */
			StructureEvents& operator=(const StructureEvents& rhs);

		private:

			// The location pointer, 4 byte
			const float *locationPtr;

			// The time pointer, 4 byte
			const float *timePtr;

			// The type pointer, 1 byte. 0 := Birth, 1 := Death, 2 := Merge, 3 := Split as in shader.
			const uint8_t *typePtr;

			// The stride.
			unsigned int stride;

			// The agglomeration.
			glm::mat4 agglomeration;

			// The number of objects stored.
			uint64_t count;

			float maxTime;

		};

		/**
		 * TODO: This class is a stub!
		 */
		class StructureEventsDataCall : public core::AbstractGetData3DCall {
		public:

			/**
			 * Answer the name of the objects of this description.
			 *
			 * @return The name of the objects of this description.
			 */
			static const char *ClassName(void) {
				return "StructureEventsDataCall";
			}

			/**
			 * Answer a human readable description of this module.
			 *
			 * @return A human readable description of this module.
			 */
			static const char *Description(void) {
				return "A custom renderer.";
			}

			/**
			 * Answer the number of functions used for this call.
			 *
			 * @return The number of functions used for this call.
			 */
			static unsigned int FunctionCount(void) {
				return AbstractGetData3DCall::FunctionCount();
			}

			/**
			 * Answer the name of the function used for this call.
			 *
			 * @param idx The index of the function to return it's name.
			 *
			 * @return The name of the requested function.
			 */
			static const char * FunctionName(unsigned int idx) {
				return AbstractGetData3DCall::FunctionName(idx);
			}

			/** Ctor. */
			StructureEventsDataCall(void);

			/** Dtor. */
			virtual ~StructureEventsDataCall(void);

			/**
			 * Assignment operator.
			 * Makes a deep copy of all members. While for data these are only
			 * pointers, the pointer to the unlocker object is also copied.
			 *
			 * @param rhs The right hand side operand
			 *
			 * @return A reference to this
			 */
			StructureEventsDataCall& operator=(const StructureEventsDataCall& rhs);

			/*inline UINT32 getEventCount(void) const {
				return this->eventCount;
			};

			inline Event getEvent(UINT32 eventIndex) const {
				return eventList[eventIndex];
			};

			inline float getMaxTime() const {
				return maxTime;
			};

			// Likely obsolete.
			inline unsigned int getEventStride() const {
				return sizeof(Event);
			};*/

			StructureEvents& getEvents() {
				return this->events;
			};

		private:
			StructureEvents events;

			/*
			UINT32 eventCount;
			float maxTime;
			std::vector<Event> eventList;
			*/
		};

		/** Description class typedef */
		typedef core::factories::CallAutoDescription<StructureEventsDataCall>
			StructureEventsDataCallDescription;

	} /* namespace mmvis_static */
} /* namespace megamol */

#endif /* MMVISSTATIC_StructureEventsDataCall_H_INCLUDED */