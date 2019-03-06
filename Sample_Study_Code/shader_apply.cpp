#include <iostream>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <freeglut.h>

int g_window_w = 500;
int g_window_h = 500;
GLhandleARB h_vertex, h_frag, h_program;

void LightInit() {
	GLfloat global_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

	GLfloat light0_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat light1_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light1_diffuse[] = { 0.5, 0.2, 0.3, 1.0 };
	GLfloat light1_specular[] = { 0.0, 0.0, 0.0, 1.0 };

	GLfloat material_ambient[] = { 0.3, 0.3, 0.3, 1.0 }; //물체 특성
	GLfloat material_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat material_specular[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat material_shininess[] = { 25.0 };

	glShadeModel(GL_SMOOTH); //구로 셰이딩 설정
	glEnable(GL_DEPTH_TEST); //깊이 버퍼 활성화
	glEnable(GL_LIGHTING); //조명 기능 활성화

	glEnable(GL_LIGHT0); //0번 광원 활성화.광원 특성
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

	glEnable(GL_LIGHT1); //1번 광원 활성화/광원 특성
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse); //물체 특성
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); //전역 주변광
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); //경면광 근접시점
}

void display2() {
	GLfloat LightPosition0[] = { 0.0, 0.0, 2.0, 1.0 }; //0번 광원 위치
	GLfloat LightPosition1[] = { 1.0, 1.0, 1.0, 1.0 }; //1번 광원 위치
	GLfloat LightDirection1[] = { -0.5, -1.0, -1.0 }; //1번 광원 방향
	GLfloat SpotAngle1[] = { 20.0 }; //1번 광원 스포트라이트 각도
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // 모델 뷰 행렬
	glLoadIdentity(); //항등 행렬 로드
	gluLookAt(0.5, 0.5, 0.5, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0); //시점 설정
	glTranslatef(0.3, 0.3, 0.0); // 모델 위치 변환
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0); //위치성 광원
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1); //스포트 라이트
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LightDirection1); //방향
	glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, SpotAngle1); //각도
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1.0); //코사인 승수
	glutSolidTorus(0.3, 0.6, 200, 200); //원환체 정의
	glutSwapBuffers();
	glFlush();
}

char* SourceRead(const char* fileName) {
	FILE* p_Sourcefile = NULL;
	char* contents = NULL;
	int length = 0;
	if (fileName != NULL) {
		int tempfile = _open(fileName, _A_RDONLY);
		length = _lseek(tempfile, 0, SEEK_END);

		_close(tempfile);
		p_Sourcefile = fopen(fileName, "rt");
		if (p_Sourcefile != NULL) {
			if (0 < length) {
				contents = (char*)malloc(sizeof(char) * (length + 1));
				length = fread(contents, sizeof(char), length, p_Sourcefile);
				contents[length] = '\0';
			}
			fclose(p_Sourcefile);
		}
	}
	return contents;
}

void InitShaders() {
	char *vertex_source, *frag_source;
	h_vertex = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB); //쉐이더 객체 생성
	h_frag = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	vertex_source = SourceRead("vertex.glsl");
	frag_source = SourceRead("frag.glsl");
	const char* p_vertex_source = vertex_source;
	const char* p_frag_source = frag_source;
	glShaderSourceARB(h_vertex, 1, &p_vertex_source, NULL); //쉐이더 객체에 소스 코드 연결
	glShaderSourceARB(h_frag, 1, &p_frag_source, NULL);
	free(vertex_source);
	free(frag_source);
	glCompileShaderARB(h_vertex); //버텍스 쉐이더 컴파일 + 오류 확인
	{
		GLint compiled;
		glGetShaderiv(h_vertex, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(h_vertex, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(h_vertex, len, &len, log);
			std::cout << "**vertex glsl error : " << std::endl;
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return;
		}
	}
	glCompileShaderARB(h_frag); //프래그먼트 쉐이더 컴파일 + 오류 확인
	{
		GLint compiled;
		glGetShaderiv(h_frag, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(h_frag, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetShaderInfoLog(h_frag, len, &len, log);
			std::cout << "**frag glsl error : " << std::endl;
			std::cerr << "Shader compilation failed: " << log << std::endl;
			delete[] log;
			return;
		}
	}
	h_program = glCreateProgramObjectARB(); //쉐이더 프로그램 생성

	glAttachObjectARB(h_program, h_vertex); //생성된 쉐이더 객체를 프로그램에 첨부
	glAttachObjectARB(h_program, h_frag);
	glLinkProgramARB(h_program); //프로그램을 링크
	glUseProgramObjectARB(h_program); //프로그램을 메모리로 로드
}

void Init() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glEnable(GL_AUTO_NORMAL);
	LightInit();
	InitShaders();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(g_window_w, g_window_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Open");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	glutDisplayFunc(display2);
	glewInit();
	if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
		Init();
		std::cout << "It's OK" << std::endl;
		glutMainLoop();
	}
	else {
		std::cout << "fucking opengl" << std::endl;
	}

	return 0;
}
