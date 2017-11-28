#include "ModelDisplayGL.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QTextStream>
#include <QTextStream>
#include <QFile>

int wheelDelta = 0;

GLDisplayer::GLDisplayer()
{
	Reset();
	mode = DefaultMode;
	setFocusPolicy(Qt::ClickFocus);
	persMode = true;
}

QSize GLDisplayer::minimumSizeHint() const
{
		return size;
}

QSize GLDisplayer::sizeHint() const
{
		return size;
}

void GLDisplayer::ScreenShot(QString filename)
{
	qDebug() << "GLDisplayer::ScreenShot" << filename;
	unsigned char* data = new unsigned char[4*width()*height()];
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadPixels(this->pos().x(), this->pos().y(), width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, data);
	QImage image(data, width(), height(), QImage::Format_RGBA8888);
	image = image.mirrored();
	bool result = image.save(filename);
	if(result)
	{
		qDebug() << "GLDisplayer::ScreenShot " << filename << "saved!" << endl;
	}
	else
	{
		qDebug() << "GLDisplayer::ScreenShot " << filename << "failed!" << endl;
	}
  delete[] data;
}

void GLDisplayer::SetOrtho()
{
	persMode = false;
	emit ProjectionMode(persMode);
	resizeGL(width(), height());
}

void GLDisplayer::SetPersPective()
{
	persMode=true;
	emit ProjectionMode(persMode);
	resizeGL(width(), height());
}

void GLDisplayer::ChangeProjectionMode()
{
	persMode = !persMode;
	emit ProjectionMode(persMode);
	qDebug() << "persMode=" << persMode;
	resizeGL(width(), height());
}

void GLDisplayer::GetProjectionMode()
{
	emit ProjectionMode(persMode);
}

void GLDisplayer::initializeGL()
{
	QFile shaderLog("shader_program_log.txt");
	shaderLog.open(QFile::WriteOnly);
	QTextStream logFile(&shaderLog);
	// Initialize OpenGL Backend
	initializeOpenGLFunctions();
	glEnable(GL_RGBA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	QObject::connect(context(), SIGNAL(aboutToBeDestroyed()), this, SLOT(teardownGL()), Qt::DirectConnection);
	QObject::connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));
	printVersionInformation();

	qDebug() << "Set global information";
	glDisable(GL_CULL_FACE);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Application-specific initialization
	{
		logFile << "Create Shader (Do not release until VAO is created)";
		qDebug() << "Create Shader (Do not release until VAO is created)";
		m_program = new QOpenGLShaderProgram();

		logFile << "Add vertex shader" << endl;
		m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/simple.vert");
		logFile << m_program->log() << endl;

		logFile << "Add fragment shader" << endl;
		m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
		logFile << m_program->log() << endl;

		logFile << "link shader" << endl;
		m_program->link();
		logFile << m_program->log() << endl;

		logFile << "bind shader" << endl;
		m_program->bind();
		logFile << m_program->log() << endl;

		qDebug() << "Cache Uniform Locations";

		// Create Buffer (Do not release until VAO is created)
		m_vertex.create();
		m_vertex.bind();
		m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		m_vertex.allocate(0, 0 * sizeof(GLfloat));

		qDebug() << "Create Vertex Array Object";
//		m_object.create();
//		m_object.bind();
		m_program->bindAttributeLocation("vertex", 0);
		m_program->bindAttributeLocation("ambient", 1);
		m_program->bindAttributeLocation("diffuse", 2);
		m_program->bindAttributeLocation("specular", 3);
		m_program->bindAttributeLocation("normal", 4);
		m_program->bindAttributeLocation("texcoord", 5);

		qDebug() << "Release (unbind) all";
//		m_object.release();
//		m_vertex.release();
//		m_program->release();
		qDebug() << "Released!";
	}
}

void GLDisplayer::resizeGL(int width, int height)
{
	m_projection.setToIdentity();
	if(persMode)
	{
		m_projection.perspective(60.0, width / float(height), 0.1, 100);
	}
	else
	{
		m_projection.ortho(-1, 1, -1, 1, -10, 10);
	}
}

void GLDisplayer::paintGL()
{
	//qDebug() << endl << "paintGL()";
	//qDebug() << "Clear";
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//qDebug() << "bind program and setUniformValue";
	m_program->bind();

  m_program->enableAttributeArray(0); // position
  m_program->enableAttributeArray(1); // ambient
  m_program->enableAttributeArray(2); // diffuse
  m_program->enableAttributeArray(3); // specular
  m_program->enableAttributeArray(4); // normal
  m_program->enableAttributeArray(5); // texcoord
	m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 17*sizeof(GLfloat));
	m_program->setAttributeBuffer(1, GL_FLOAT, 3*sizeof(GLfloat), 3, 17*sizeof(GLfloat));
	m_program->setAttributeBuffer(2, GL_FLOAT, 6*sizeof(GLfloat), 3, 17*sizeof(GLfloat));
	m_program->setAttributeBuffer(3, GL_FLOAT, 9*sizeof(GLfloat), 3, 17*sizeof(GLfloat));
	m_program->setAttributeBuffer(4, GL_FLOAT, 12*sizeof(GLfloat), 3, 17*sizeof(GLfloat));
	m_program->setAttributeBuffer(5, GL_FLOAT, 15*sizeof(GLfloat), 2, 17*sizeof(GLfloat));

	m_program->setUniformValue("worldToCamera", m_camera.toMatrix());
	m_program->setUniformValue("cameraToView", m_projection);
	{
		Light light;
		//light.SetPointLight(QVector3D(3, 3, 3), 5.0, QVector3D(1.0, 1.0, 1.0));
		light.SetEnvLight();
//		m_object.bind();
		m_program->setUniformValue("modelToWorld", m_transform.toMatrix());
		m_program->setUniformValue("lightInfo.type", light.type);
		m_program->setUniformValue("lightInfo.position", light.pos);
		m_program->setUniformValue("lightInfo.color", light.color);
		m_program->setUniformValue("lightInfo.direction", light.direction);
		m_program->setUniformValue("lightInfo.angle2", light.angle2);
		m_program->setUniformValue("lightInfo.strength", light.strength);
		m_program->setUniformValue("rotation", m_transform.rotation().toRotationMatrix());
		m_program->setUniformValue("cameraPos", m_camera.translation());
		m_program->setUniformValue("texture", 0);
		m_program->setUniformValue("mode", mode);
		//qDebug() << "Draw";

    if(obj.data() == nullptr)
    {
      for(int mid = 0; mid < obj->MeshNum(); mid++)
      {
        BRepMP mesh = obj->Mesh(mid);
        for(int fid = 0; fid < mesh->FaceNum(); fid++)
        {
          BRepFP face = mesh->Face(fid);
          // draw stencil buffer first
          glEnable(GL_STENCIL_TEST);
          glStencilFunc(GL_NEVER, 0, 1);
          glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);
          for(int lid = 0; lid<face->LoopNum(); lid++)
          {
            BRepLP loop = face->Loop(lid);
            QVector<float> vertData;
            for(int heid=0; heid<loop->HalfEdgeNum(); heid++)
            {
              BRepHEP he = loop->HalfEdge(heid);
              QVector3D pos = he->From()->Position();
              vertData.push_back(pos.x());
              vertData.push_back(pos.y());
              vertData.push_back(pos.z());

              // ambient
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // diffuse
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // specular
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // normal
              QVector3D normal = face->Normal();
              vertData.push_back(normal.x()); vertData.push_back(normal.y()); vertData.push_back(normal.z());

              // texcoords
              vertData.push_back(0.0); vertData.push_back(0.0);
            }

            m_vertex.release();
            m_vertex.create();
            m_vertex.allocate(vertData, vertData.count());
            m_vertex.bind();
            glDrawArrays(GL_POLYGON, 0, vertData.count());
          }

          glStencilFunc(GL_EQUAL, 1, 1);
          glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

          for(int lid = 0; lid<face->LoopNum(); lid++)
          {
            BRepLP loop = face->Loop(lid);
            if(loop->Dir()) continue;
            QVector<float> vertData;
            for(int heid=0; heid<loop->HalfEdgeNum(); heid++)
            {
              BRepHEP he = loop->HalfEdge(heid);
              QVector3D pos = he->From()->Position();
              vertData.push_back(pos.x());
              vertData.push_back(pos.y());
              vertData.push_back(pos.z());

              // ambient
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // diffuse
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // specular
              vertData.push_back(1.0); vertData.push_back(1.0); vertData.push_back(1.0);

              // normal
              QVector3D normal = face->Normal();
              vertData.push_back(normal.x()); vertData.push_back(normal.y()); vertData.push_back(normal.z());

              // texcoords
              vertData.push_back(0.0); vertData.push_back(0.0);
            }

            m_vertex.release();
            m_vertex.create();
            m_vertex.allocate(vertData, vertData.count());
            m_vertex.bind();
            glDrawArrays(GL_POLYGON, 0, vertData.count());
          }
        }
      }
    }
	}
	m_program->release();
	//qDebug() << "Released";
}

void GLDisplayer::teardownGL()
{
	// Actually destroy our OpenGL information
//	m_object.destroy();
	m_vertex.destroy();
	delete m_program;
}

void GLDisplayer::Reset()
{
	m_transform.setRotation(0, 0, 0, 0);
	m_transform.setTranslation(0, 0, 0);
}

void GLDisplayer::update()
{
	const float cMinRo = 0.0;
	const float cMaxRo = 5.0;
	// Update input
	Input::update();

	float rotSpeed   = 0.1f;
	float transSpeed = 0.01f;
	if (Input::keyPressed(Qt::Key_Shift))
	{
		transSpeed = 0.005f;
		rotSpeed = 0.005f;
	}

	// Camera Transformation
	if (Input::buttonPressed(Qt::LeftButton))
	{

		// Handle rotations
//		camera.setX(camera.x()-rotSpeed * Input::mouseDelta().x());
//		camera.setY(camera.y()+rotSpeed * Input::mouseDelta().y());
		m_transform.rotate(Input::mouseDelta().x()*rotSpeed, 0, 1, 0);
		m_transform.rotate(Input::mouseDelta().y()*rotSpeed, 1, 0, 0);
	}

	// Handle translations
	QVector3D translation;
	if (Input::keyPressed(Qt::Key_W))
	{
		translation = QVector3D(0, 0, -transSpeed);
	}
	if (Input::keyPressed(Qt::Key_S))
	{
		translation = QVector3D(0, 0, transSpeed);
	}
	translation-=QVector3D(0, 0, wheelDelta*transSpeed/10);
	wheelDelta=0;

//	camera=camera+translation;

	if (Input::keyPressed(Qt::Key_A))
	{
//		camera.setX(camera.x()+transSpeed);
		m_transform.rotate(-rotSpeed, 0, 1, 0);
	}
	if (Input::keyPressed(Qt::Key_D))
	{
//		camera.setX(camera.x()-transSpeed);
		m_transform.rotate(rotSpeed, 0, 1, 0);
	}
	if (Input::keyPressed(Qt::Key_Q))
	{
//		camera.setY(camera.y()+transSpeed);
		m_transform.rotate(-rotSpeed, 1, 0, 0);
	}
	if (Input::keyPressed(Qt::Key_E))
	{
//		camera.setY(camera.y()-transSpeed);
		m_transform.rotate(rotSpeed, 1, 0, 0);
	}

//	if (Input::keyPressed(Qt::Key_R))
//	{
//		camera=QVector3D(0, 0, 1);
//	}

	if((m_camera.translation()+translation).z() > cMaxRo) translation.setZ(cMaxRo-m_camera.translation().z());
	if((m_camera.translation()+translation).z() < cMinRo) translation.setZ(cMinRo-m_camera.translation().z());
	m_camera.translate(translation);

//	if(camera.z()>cMaxRo) camera.setZ(cMaxRo);
//	if(camera.z()<cMinRo) camera.setZ(cMinRo);
//	if(camera.y()>M_PI/2) camera.setY(M_PI/2);
//	if(camera.y()<-M_PI/2) camera.setY(-M_PI/2);

//	m_camera.setTranslation(0, 0, 0);
//	m_camera.setRotation(0, 0, 0, 0);

//	QVector3D pos(
//				camera.z()*cos(camera.y())*sin(camera.x()),
//				camera.z()*sin(camera.y()),
//				camera.z()*cos(camera.y())*cos(camera.x()));
//	m_camera.translate(pos);

//	m_camera.rotate(camera.x()/M_PI*180, Camera3D::LocalUp);
//	m_camera.rotate(-camera.y()/M_PI*180, m_camera.right());
	// Schedule a redraw
	QOpenGLWidget::update();
}

void GLDisplayer::keyPressEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyPress(event->key());
	}
}

void GLDisplayer::keyReleaseEvent(QKeyEvent *event)
{
	if (event->isAutoRepeat())
	{
		event->ignore();
	}
	else
	{
		Input::registerKeyRelease(event->key());
	}
}

void GLDisplayer::mousePressEvent(QMouseEvent *event)
{
	Input::registerMousePress(event->button());
}

void GLDisplayer::mouseReleaseEvent(QMouseEvent *event)
{
	Input::registerMouseRelease(event->button());
}

void GLDisplayer::wheelEvent(QWheelEvent *event)
{
	wheelDelta = event->delta();
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void GLDisplayer::printVersionInformation()
{
	QString glType;
	QString glVersion;
	QString glProfile;

	// Get Version Information
	glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
	glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

	// Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
	switch (format().profile())
	{
		CASE(NoProfile);
		CASE(CoreProfile);
		CASE(CompatibilityProfile);
	}
#undef CASE

	// qPrintable() will print our QString w/o quotes around it.
	qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void GLDisplayer::SetDisplayMode(int dm)
{
	mode = dm;
}

void GLDisplayer::SetSizeToHint()
{
	resize(sizeHint());
}

void GLDisplayer::CameraSetRotation(float angle, float x, float y, float z)
{
	m_camera.setRotation(angle, x, y, z);
}

void GLDisplayer::CameraRotate(float angle, float x, float y, float z)
{
	m_camera.rotate(angle, x, y, z);
}

void GLDisplayer::CameraRotation()
{

}

void GLDisplayer::CameraSetTranslation(float x, float y, float z)
{
	m_camera.setTranslation(x, y, z);
}

void GLDisplayer::CameraTranslate(float x, float y, float z)
{
	m_camera.translate(x, y, z);
}

void GLDisplayer::CameraTranslation()
{

}

void GLDisplayer::WorldSetRotation(float angle, float x, float y, float z)
{
	m_transform.setRotation(angle, x, y, z);
}

void GLDisplayer::WorldRotate(float angle, float x, float y, float z)
{
	m_transform.rotate(angle, x, y, z);
}

void GLDisplayer::WorldRotation()
{

}

void GLDisplayer::WorldSetTranslation(float x, float y, float z)
{
	m_transform.setTranslation(x, y, z);
}

void GLDisplayer::WorldTranslate(float x, float y, float z)
{
	m_transform.translate(x, y, z);
}

void GLDisplayer::WorldTranslation()
{

}

void GLDisplayer::SetSize(int size)
{
    qDebug() << "GLDisplayer::SetSize" << QSize(size, size);
	this->size = QSize(size, size);
	setFixedSize(QSize(size, size));
}

bool GLDisplayer::SetBRepModel(BRepOP object)
{
  if(!object)
  {
    return false;
  }

  obj = object;
}
