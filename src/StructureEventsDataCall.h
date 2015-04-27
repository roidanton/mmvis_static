/**
 * StructureEventsDataCall.h
 *
 * Copyright (C) 2009-2015 by MegaMol Team
 * Copyright (C) 2015 by Richard H�hne, TU Dresden
 * Alle Rechte vorbehalten.
 */

#ifndef MMVISSTATIC_StructureEventsDataCall_H_INCLUDED
#define MMVISSTATIC_StructureEventsDataCall_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "mmcore/AbstractGetData3DCall.h"
#include "mmcore/factories/CallAutoDescription.h"
#include "glm/glm/glm.hpp"

namespace megamol {
	namespace mmvis_static {

		/**
		 * One event contains a type, a position and a time as well as an agglomeration matrix.
		 */
		class StructureEvent {
		public:
			/** Possible values for the event type */
			enum EventType {
				BIRTH,
				DEATH,
				MERGE,
				SPLIT
			};

			/**
			* Answer the event type
			*
			* @return The event type
			*/
			inline EventType getEventType(void) const {
				return this->type;
			};

			/**
			* Answer the position pointer
			*
			* @return The position pointer
			*/
			inline glm::vec3 getPosition(void) const {
				return this->position;
			};

			/**
			* Sets the event type
			*/
			inline void setEventType(EventType eventType) {
				this->type = eventType;
			};

		private:

			/** The agglomeration. */
			glm::mat4 agglomeration;

			/** The event type. */
			EventType type;

			/** The position pointer. */
			glm::vec3 position;

			/** The time step. */
			unsigned int timeStep;
		};

		/**
		 * Container for all structure events. One event contains a type, a position and a time.
		 */
		class StructureEvents {
		public:
			/** Ctor */
			StructureEvents(void);

			/**
			 * Copy ctor
			 * @param src The object to clone from
			 */
			StructureEvents(const StructureEvents& src);

			/** Dtor */
			~StructureEvents(void);
			
		private:
			unsigned int numberOfEvents;
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

			/** typedef for legacy name */
			typedef StructureEvents Events;

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

		private:

		};

		/** Description class typedef */
		typedef core::factories::CallAutoDescription<StructureEventsDataCall>
			StructureEventsDataCallDescription;

	} /* namespace mmvis_static */
} /* namespace megamol */

#endif /* MMVISSTATIC_StructureEventsDataCall_H_INCLUDED */