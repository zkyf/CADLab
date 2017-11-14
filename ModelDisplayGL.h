#pragma once

#ifndef MODELDISPLAY_H
#define MODELDISPLAY_H

#include <QGuiApplication>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QPainter>
#include <QOpenGLVertexArrayObject>
#include "camera3d.h"
#include "transform3d.h"
#include "input.h"
#include "light.h"

#include "brep.hpp"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

struct RenderFaceInfo
{
  int vertNum = 0;
  QString texturePath = "";
};

class GLDisplayer : public QOpenGLWidget, protected QOpenGLFunctions
{
		Q_OBJECT

		// OpenGL Events
		public:

		enum DisplayMode
		{
			DefaultMode = 0,
			WhiteMode = 1,
			NormalMode = 2
		};
			GLDisplayer();
			void initializeGL();
			void resizeGL(int width, int height);
			void paintGL();
			QSize minimumSizeHint() const override;
			QSize sizeHint() const override;
      bool SetBRepModel(BRepOP);

	public slots:
			void SetSize(int size);
			void Reset();
			void ScreenShot(QString filename);

			// set display options
			void SetDisplayMode(int dm);
			void SetOrtho();
			void SetPersPective();
			void ChangeProjectionMode();
			void GetProjectionMode();
			void SetSizeToHint();

			// camera control
			void CameraSetRotation(float, float, float, float);
			void CameraRotate(float, float, float, float);
			void CameraRotation();
			void CameraSetTranslation(float, float, float);
			void CameraTranslate(float, float, float);
			void CameraTranslation();

			// world control
			void WorldSetRotation(float, float, float, float);
			void WorldRotate(float, float, float, float);
			void WorldRotation();
			void WorldSetTranslation(float, float, float);
			void WorldTranslate(float, float, float);
			void WorldTranslation();

	signals:
			bool ProjectionMode(bool);

		protected slots:
			void teardownGL();
			void update();

		protected:
			void keyPressEvent(QKeyEvent *event);
			void keyReleaseEvent(QKeyEvent *event);
			void mousePressEvent(QMouseEvent *event);
			void mouseReleaseEvent(QMouseEvent *event);
			void wheelEvent(QWheelEvent *event);

		private:
			QSize size = QSize(512, 512);

			// OpenGL State Information
			QOpenGLBuffer m_vertex;
			QOpenGLShaderProgram *m_program;

			// Shader Information
			bool persMode;
			int mode;
			QMatrix4x4 m_projection;
			int faceCount;

			QVector<QOpenGLTexture*> textures;
      QVector<RenderFaceInfo> renderFaceInfo;

			Camera3D m_camera;
			Transform3D m_transform;

			// Private Helpers
			void printVersionInformation();
};
#endif
