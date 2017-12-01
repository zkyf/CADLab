#include "ModelDisplayGL.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QTextStream>
#include <QTextStream>
#include <QFile>

int wheelDelta = 0;

GLDisplayer::GLDisplayer() :
  obj(nullptr)
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
  glClearColor(0, 0, 0, 0);
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
    light.SetPointLight(m_camera.translation(), 5.0, QVector3D(1.0, 1.0, 1.0));
//    light.SetDirectionalLight(QVector3D(1, 1, 1));
//		light.SetEnvLight();
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
    m_program->setUniformValue("mode", 0);
    //

    static bool init = true;
    if(obj != nullptr)
    {
      if(init)
      {
        init = false;
//        BRepMP mesh = obj->Mesh(0);
//        for(int fid=0; fid<mesh->FaceNum(); fid++)
//        {
//          BRepFP face = mesh->Face(fid);
//          qDebug() << "==================";
//          qDebug() << "face num = " << mesh->FaceNum() << " #" << fid << "add=" << face;
//          qDebug() << "normal = " << face->Normal();
//          qDebug() << "face half edges: ";
//          for(int i=0; i<face->LoopNum(); i++)
//          {
//            qDebug() << "  loop#" << i << " face add=" << face->Loop(i)->Face() << " loop add=" << face->Loop(i);
//            BRepLP l = face->Loop(i);
//            for(int j=0; j<l->HalfEdgeNum(); j++)
//            {
//              BRepHEP he = l->HalfEdge(j);
//              qDebug() << "    he#" << j << "from " << he->From()->Position() << " to " << he->To()->Position() << " loop add=" << he->Loop();
//            }
//          }
//        }
      }
//      qDebug() << "Draw";
      for(int mid = 0; mid < obj->MeshNum(); mid++)
      {
//        qDebug() << "";
//        qDebug() << "mid=" << mid;
        BRepMP mesh = obj->Mesh(mid);
        for(int fid = 0; fid < mesh->FaceNum(); fid++)
        {
//          qDebug() << "fid=" << fid;
          BRepFP face = mesh->Face(fid);
          if(!face->IsPlane()) continue;
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
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // diffuse
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // specular
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // normal
              QVector3D normal = face->Normal();
              vertData.push_back(normal.x()); vertData.push_back(normal.y()); vertData.push_back(normal.z());

              // texcoords
              vertData.push_back(0.0); vertData.push_back(0.0);
            }

            m_vertex.release();
            m_vertex.create();
            m_vertex.allocate(vertData.data(), vertData.count());
            m_vertex.bind();
            glDrawArrays(GL_POLYGON, 0, vertData.count());
          }

          glStencilFunc(GL_ALWAYS, 1, 1);
          glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

          for(int lid = 0; lid<face->LoopNum(); lid++)
          {
            BRepLP loop = face->Loop(lid);
//            qDebug() << "lid=" << lid << loop->HalfEdgeNum();
            QVector<float> vertData;
            for(int heid=0; heid<loop->HalfEdgeNum(); heid++)
            {
//              qDebug() << "heid=" << heid;
              BRepHEP he = loop->HalfEdge(heid);
              QVector3D pos = he->From()->Position();
              vertData.push_back(pos.x());
              vertData.push_back(pos.y());
              vertData.push_back(pos.z());

              // ambient
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // diffuse
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // specular
              vertData.push_back(1); vertData.push_back(1); vertData.push_back(1);

              // normal
              QVector3D normal = face->Normal();
              vertData.push_back(normal.x()); vertData.push_back(normal.y()); vertData.push_back(normal.z());

              // texcoords
              vertData.push_back(0.0); vertData.push_back(0.0);
            }

//            qDebug() << "loop->HalfEdgeNum() = " << loop->HalfEdgeNum();
            m_vertex.release();
            m_vertex.create();
            m_vertex.bind();
            m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
            m_vertex.allocate(vertData.constData(), vertData.count()*sizeof(GLfloat));
            glDrawArrays(GL_POLYGON, 0, loop->HalfEdgeNum());
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
	// Update input
	Input::update();

  float rotSpeed   = 0.1f;
  float transSpeed = 0.1f;
	if (Input::keyPressed(Qt::Key_Shift))
	{
    transSpeed = 0.05f;
		rotSpeed = 0.005f;
	}

	// Camera Transformation
	if (Input::buttonPressed(Qt::LeftButton))
	{

		// Handle rotations
//		camera.setX(camera.x()-rotSpeed * Input::mouseDelta().x());
//		camera.setY(camera.y()+rotSpeed * Input::mouseDelta().y());
    m_camera.rotate(Input::mouseDelta().x()*rotSpeed, m_camera.up());
    m_camera.rotate(Input::mouseDelta().y()*rotSpeed, m_camera.right());
	}

	// Handle translations
	QVector3D translation;
	if (Input::keyPressed(Qt::Key_W))
	{
    translation = transSpeed*m_camera.forward();
	}
	if (Input::keyPressed(Qt::Key_S))
	{
    translation = -transSpeed*m_camera.forward();
	}
	if (Input::keyPressed(Qt::Key_A))
	{
    translation = -transSpeed*m_camera.right();
	}
	if (Input::keyPressed(Qt::Key_D))
	{
    translation = transSpeed*m_camera.right();
	}
	if (Input::keyPressed(Qt::Key_Q))
	{
    translation = transSpeed*m_camera.up();
	}
	if (Input::keyPressed(Qt::Key_E))
	{
    translation = -transSpeed*m_camera.up();
	}

	m_camera.translate(translation);

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
