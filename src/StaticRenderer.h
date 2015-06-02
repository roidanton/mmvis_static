/**
 * StaticRenderer.h
 *
 * Copyright (C) 2009-2015 by MegaMol Team
 * Alle Rechte vorbehalten.
 */

#ifndef MMVISSTATIC_STATICRENDERER_H_INCLUDED
#define MMVISSTATIC_STATICRENDERER_H_INCLUDED
#if (defined(_MSC_VER) && (_MSC_VER > 1000))
#pragma once
#endif /* (defined(_MSC_VER) && (_MSC_VER > 1000)) */

#include "mmcore/view/Renderer3DModule.h"
#include "mmcore/Call.h"
#include "mmcore/CallerSlot.h"
#include "mmcore/param/ParamSlot.h"
#include "mmcore/param/EnumParam.h"
#include "StructureEventsDataCall.h"
#include "vislib/graphics/gl/GLSLShader.h"
#include "vislib/graphics/gl/OpenGLTexture2D.h"
#include "glm/glm/glm.hpp"

namespace megamol {
	namespace mmvis_static {

		/**
		 * Cares about visual attributes.
		 */
		class VisualAttributes {
		public:
			enum class AttributeType : int {
				Brightness,
				Hue,
				Opacity,
				Position,
				// Contains no separate position x,y,z since the goal is an overlay
				// with the actual data so the event position has to be
				// attached to the visual position attribute.
				Size,
				Texture
			};
			enum class ParameterType : int {
				Agglomeration,
				Location,
				Type,
				Time
			};

			/// Ctor.
			VisualAttributes(void);

			/// Dtor.
			virtual ~VisualAttributes(void);

			/**
			 * Fill an EnumParam with valid visual attributes depending on parameter.
			 * @param attributes The EnumParam to fill.
			 * @param parameterType A parameter type.
			 */
			static void getValidAttributes(core::param::EnumParam *attributes, ParameterType parameterType);

			/**
			 * Gets the AttributeType enum from a wchar_t string.
			 * @see http://www.codeproject.com/Articles/76252/What-are-TCHAR-WCHAR-LPSTR-LPWSTR-LPCTSTR-etc
			 *
			 * @param attribute A wchar_t pointer with the char sequence of an visual attribute.
			 * @return The Attribute type.
			 */
			static AttributeType getAttributeType(wchar_t* attribute);

			/**
			 * Gets the AttributeType enum from a EnumParam.
			 *
			 * @param parameterSlot Expects an EnumParam!
			 * @return The Attribute type.
			 */
			static AttributeType getAttributeType(core::param::ParamSlot *attributeSlot);

			static void getAttributeValue(AttributeType attr, ParameterType param);
		};

		/**
		 * The Renderer.
		 */
		class StaticRenderer : public core::view::Renderer3DModule {
		public:
			/**
			 * Answer the name of this module.
			 *
			 * @return The name of this module.
			 */
			static const char *ClassName(void) {
				return "StaticRenderer";
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
			 * Answers whether this module is available on the current system.
			 *
			 * @return 'true' if the module is available, 'false' otherwise.
			 */
			static bool IsAvailable(void) {
#ifdef _WIN32
#if defined(DEBUG) || defined(_DEBUG)
				HDC dc = ::wglGetCurrentDC();
				HGLRC rc = ::wglGetCurrentContext();
				ASSERT(dc != NULL);
				ASSERT(rc != NULL);
#endif // DEBUG || _DEBUG
#endif // _WIN32
				return vislib::graphics::gl::GLSLShader::AreExtensionsAvailable();
			}

			/// Ctor.
			StaticRenderer(void);

			/// Dtor.
			virtual ~StaticRenderer(void);

		protected:
			/**
			 * Implementation of 'Create'.
			 *
			 * @return 'true' on success, 'false' otherwise.
			 */
			virtual bool create(void);

			/**
			 * The get capabilities callback. The module should set the members
			 * of 'call' to tell the caller its capabilities.
			 *
			 * @param call The calling call.
			 *
			 * @return The return value of the function.
			 */
			virtual bool GetCapabilities(core::Call& call);

			/**
			 * The get extents callback. The module should set the members of
			 * 'call' to tell the caller the extents of its data (bounding boxes
			 * and times).
			 *
			 * @param call The calling call.
			 *
			 * @return The return value of the function.
			 */
			virtual bool GetExtents(core::Call& call);

			/**
			 * Implementation of 'Release'.
			 */
			virtual void release(void);

			/**
			 * The render callback.
			 *
			 * @param call The calling call.
			 *
			 * @return The return value of the function.
			 */
			virtual bool Render(core::Call& call);

			/**
			 * Gets the data from a call.
			 * TODO: Write own eventDataCall to submit event data for
			 * custom shapes, colors etc for the element.
			 */
			StructureEventsDataCall *getData(unsigned int t, float& outScaling);

			/**
			 * TODO: Document
			 *
			 * @param clipDat points to four floats
			 * @param clipCol points to four floats
			 */
			void getClipData(float *clipDat, float *clipCol);

		private:
			/**
			 * Loads a png texture from file system and creates OGL2 texture in memory.
			 *
			 * Currently unused since png loader doesnt work and we have CreateOGLTexture.
			 * However this function uses a filenameSlot which should be the way to go.
			 */
			void LoadPngTexture(core::param::ParamSlot *filenameSlot, vislib::graphics::gl::OpenGLTexture2D &ogl2Texture);
			
			/**
			 * Loads a png texture from file system using lodePNG and creates OGL texture in memory.
			 */
			void CreateOGLTextureFromFile(char* filename, GLuint &textureID);

			/// The filepathes for the textures. Currently unused!
			core::param::ParamSlot filePathBirthTextureSlot;
			core::param::ParamSlot filePathDeathTextureSlot;
			core::param::ParamSlot filePathMergeTextureSlot;
			core::param::ParamSlot filePathSplitTextureSlot;

			/// The visual attributes for events.
			core::param::ParamSlot eventAgglomerationVisAttrSlot;
			core::param::ParamSlot eventLocationVisAttrSlot;
			core::param::ParamSlot eventTypeVisAttrSlot;
			core::param::ParamSlot eventTimeVisAttrSlot;

			/// The eventtype textures. Obsolete, replaced by IDs. MegaMol configurator doesn't like them anyways.
			/*vislib::graphics::gl::OpenGLTexture2D birthOGL2Texture;
			vislib::graphics::gl::OpenGLTexture2D deathOGL2Texture;
			vislib::graphics::gl::OpenGLTexture2D mergeOGL2Texture;
			vislib::graphics::gl::OpenGLTexture2D splitOGL2Texture;*/

			/// The shader for the 3DSprite/Billboard
			vislib::graphics::gl::GLSLShader billboardShader;

			/// Containing all data for the vertex shader.
			struct Vertex {
				glm::vec3 position; ///< The position of the event.
				glm::vec2 spanQuad; ///< Generate quads in shader by translating the vertex by this vector.
				glm::vec2 texUV; ///< Standard assignment of UV.
				GLfloat eventType; ///< Eventtype hardcoded in shader: 0 = birth, 1 = death, 2 = merge, 3 = split
				glm::vec3 colorHSV; ///< Color in HSV: Hue, Saturation, Value = [0,1]. Converted to rgb in shader.
				GLfloat opacity; ///< Opacity.
			};

			/// Shader attributes.
			GLint shaderAttributeIndex_position,
				shaderAttributeIndex_spanQuad,
				shaderAttributeIndex_texUV,
				shaderAttributeIndex_eventType,
				shaderAttributeIndex_colorHSV,
				shaderAttributeIndex_opacity;
				
			/// Vertex Buffer Object index for the vertex shader.
			GLuint vbo;

			/// The call for data
			core::CallerSlot getDataSlot;

			/// The call for clipping plane
			core::CallerSlot getClipPlaneSlot;

			/// Detection of the first pass of the renderer.
			/// Required for initial vertex buffer creation and data retrieving.
			/// Both has to happen in Renderer since the slots are only available
			/// there.
			bool firstPass;

			/// The texture IDs. Global since only loaded once in renderer.
			GLuint textureIDs[4];

			/// Container for the data call. Global since only created once in renderer.
			StructureEventsDataCall *dataCall;
		};

	} /* namespace mmvis_static */
} /* namespace megamol */

#endif /* MMVISSTATIC_STATICRENDERER_H_INCLUDED */