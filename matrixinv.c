#include <u.h>
#include <libc.h>

typedef double Matrix[3][3];
typedef double Matrix3[4][4];

void
identity(Matrix m)
{
	memset(m, 0, 3*3*sizeof(double));
	m[0][0] = m[1][1] = m[2][2] = 1;
}

void
addm(Matrix a, Matrix b)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			a[i][j] += b[i][j];
}

void
subm(Matrix a, Matrix b)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			a[i][j] -= b[i][j];
}

void
mulm(Matrix a, Matrix b)
{
	int i, j, k;
	Matrix tmp;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++){
			tmp[i][j] = 0;
			for(k = 0; k < 3; k++)
				tmp[i][j] += a[i][k]*b[k][j];
		}
	memmove(a, tmp, 3*3*sizeof(double));
}

void
smulm(Matrix m, double s)
{
	int i, j;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			m[i][j] *= s;
}

double
detm(Matrix m)
{
	return m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1])+
	       m[0][1]*(m[1][2]*m[2][0] - m[1][0]*m[2][2])+
	       m[0][2]*(m[1][0]*m[2][1] - m[1][1]*m[2][0]);
}

double
tracem(Matrix m)
{
	return m[0][0] + m[1][1] + m[2][2];
}

/* Cayley-Hamilton */
void
invertm(Matrix m)
{
	Matrix m², r;
	double det, trm, trm²;

	det = detm(m);
	if(det == 0)
		return; /* singular matrices are not invertible */
	trm = tracem(m);
	memmove(m², m, 3*3*sizeof(double));
	mulm(m², m²);
	trm² = tracem(m²);
	identity(r);
	smulm(r, (trm*trm - trm²)/2);
	smulm(m, trm);
	subm(r, m);
	addm(r, m²);
	smulm(r, 1/det);
	memmove(m, r, 3*3*sizeof(double));
}

/* Cramer's */
void
adjm(Matrix m)
{
	Matrix tmp;

	tmp[0][0] =  m[1][1]*m[2][2] - m[1][2]*m[2][1];
	tmp[0][1] = -m[0][1]*m[2][2] + m[0][2]*m[2][1];
	tmp[0][2] =  m[0][1]*m[1][2] - m[0][2]*m[1][1];
	tmp[1][0] = -m[1][0]*m[2][2] + m[1][2]*m[2][0];
	tmp[1][1] =  m[0][0]*m[2][2] - m[0][2]*m[2][0];
	tmp[1][2] = -m[0][0]*m[1][2] + m[0][2]*m[1][0];
	tmp[2][0] =  m[1][0]*m[2][1] - m[1][1]*m[2][0];
	tmp[2][1] = -m[0][0]*m[2][1] + m[0][1]*m[2][0];
	tmp[2][2] =  m[0][0]*m[1][1] - m[0][1]*m[1][0];
	memmove(m, tmp, 3*3*sizeof(double));
}

void
cinvertm(Matrix m)
{
	double det;

	det = detm(m);
	if(det == 0)
		return; /* singular matrices are not invertible */
	adjm(m);
	smulm(m, 1/det);
}

void
identity3(Matrix3 m)
{
	memset(m, 0, 4*4*sizeof(double));
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1;
}

void
addm3(Matrix3 a, Matrix3 b)
{
	int i, j;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			a[i][j] += b[i][j];
}

void
subm3(Matrix3 a, Matrix3 b)
{
	int i, j;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			a[i][j] -= b[i][j];
}

void
mulm3(Matrix3 a, Matrix3 b)
{
	int i, j, k;
	Matrix3 tmp;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++){
			tmp[i][j] = 0;
			for(k = 0; k < 4; k++)
				tmp[i][j] += a[i][k]*b[k][j];
		}
	memmove(a, tmp, 4*4*sizeof(double));
}

void
smulm3(Matrix3 m, double s)
{
	int i, j;

	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			m[i][j] *= s;
}

double
detm3(Matrix3 m)
{
	return m[0][0]*(m[1][1]*(m[2][2]*m[3][3] - m[2][3]*m[3][2])+
			m[1][2]*(m[2][3]*m[3][1] - m[2][1]*m[3][3])+
			m[1][3]*(m[2][1]*m[3][2] - m[2][2]*m[3][1]))
	      -m[0][1]*(m[1][0]*(m[2][2]*m[3][3] - m[2][3]*m[3][2])+
			m[1][2]*(m[2][3]*m[3][0] - m[2][0]*m[3][3])+
			m[1][3]*(m[2][0]*m[3][2] - m[2][2]*m[3][0]))
	      +m[0][2]*(m[1][0]*(m[2][1]*m[3][3] - m[2][3]*m[3][1])+
			m[1][1]*(m[2][3]*m[3][0] - m[2][0]*m[3][3])+
			m[1][3]*(m[2][0]*m[3][1] - m[2][1]*m[3][0]))
	      -m[0][3]*(m[1][0]*(m[2][1]*m[3][2] - m[2][2]*m[3][1])+
			m[1][1]*(m[2][2]*m[3][0] - m[2][0]*m[3][2])+
			m[1][2]*(m[2][0]*m[3][1] - m[2][1]*m[3][0]));
}

double
tracem3(Matrix3 m)
{
	return m[0][0] + m[1][1] + m[2][2] + m[3][3];
}

void
adjm3(Matrix3 m)
{
	Matrix3 tmp;

	tmp[0][0]=m[1][1]*(m[2][2]*m[3][3] - m[2][3]*m[3][2])+
		  m[2][1]*(m[1][3]*m[3][2] - m[1][2]*m[3][3])+
		  m[3][1]*(m[1][2]*m[2][3] - m[1][3]*m[2][2]);
	tmp[0][1]=m[0][1]*(m[2][3]*m[3][2] - m[2][2]*m[3][3])+
		  m[2][1]*(m[0][2]*m[3][3] - m[0][3]*m[3][2])+
		  m[3][1]*(m[0][3]*m[2][2] - m[0][2]*m[2][3]);
	tmp[0][2]=m[0][1]*(m[1][2]*m[3][3] - m[1][3]*m[3][2])+
		  m[1][1]*(m[0][3]*m[3][2] - m[0][2]*m[3][3])+
		  m[3][1]*(m[0][2]*m[1][3] - m[0][3]*m[1][2]);
	tmp[0][3]=m[0][1]*(m[1][3]*m[2][2] - m[1][2]*m[2][3])+
		  m[1][1]*(m[0][2]*m[2][3] - m[0][3]*m[2][2])+
		  m[2][1]*(m[0][3]*m[1][2] - m[0][2]*m[1][3]);
	tmp[1][0]=m[1][0]*(m[2][3]*m[3][2] - m[2][2]*m[3][3])+
		  m[2][0]*(m[1][2]*m[3][3] - m[1][3]*m[3][2])+
		  m[3][0]*(m[1][3]*m[2][2] - m[1][2]*m[2][3]);
	tmp[1][1]=m[0][0]*(m[2][2]*m[3][3] - m[2][3]*m[3][2])+
		  m[2][0]*(m[0][3]*m[3][2] - m[0][2]*m[3][3])+
		  m[3][0]*(m[0][2]*m[2][3] - m[0][3]*m[2][2]);
	tmp[1][2]=m[0][0]*(m[1][3]*m[3][2] - m[1][2]*m[3][3])+
		  m[1][0]*(m[0][2]*m[3][3] - m[0][3]*m[3][2])+
		  m[3][0]*(m[0][3]*m[1][2] - m[0][2]*m[1][3]);
	tmp[1][3]=m[0][0]*(m[1][2]*m[2][3] - m[1][3]*m[2][2])+
		  m[1][0]*(m[0][3]*m[2][2] - m[0][2]*m[2][3])+
		  m[2][0]*(m[0][2]*m[1][3] - m[0][3]*m[1][2]);
	tmp[2][0]=m[1][0]*(m[2][1]*m[3][3] - m[2][3]*m[3][1])+
		  m[2][0]*(m[1][3]*m[3][1] - m[1][1]*m[3][3])+
		  m[3][0]*(m[1][1]*m[2][3] - m[1][3]*m[2][1]);
	tmp[2][1]=m[0][0]*(m[2][3]*m[3][1] - m[2][1]*m[3][3])+
		  m[2][0]*(m[0][1]*m[3][3] - m[0][3]*m[3][1])+
		  m[3][0]*(m[0][3]*m[2][1] - m[0][1]*m[2][3]);
	tmp[2][2]=m[0][0]*(m[1][1]*m[3][3] - m[1][3]*m[3][1])+
		  m[1][0]*(m[0][3]*m[3][1] - m[0][1]*m[3][3])+
		  m[3][0]*(m[0][1]*m[1][3] - m[0][3]*m[1][1]);
	tmp[2][3]=m[0][0]*(m[1][3]*m[2][1] - m[1][1]*m[2][3])+
		  m[1][0]*(m[0][1]*m[2][3] - m[0][3]*m[2][1])+
		  m[2][0]*(m[0][3]*m[1][1] - m[0][1]*m[1][3]);
	tmp[3][0]=m[1][0]*(m[2][2]*m[3][1] - m[2][1]*m[3][2])+
		  m[2][0]*(m[1][1]*m[3][2] - m[1][2]*m[3][1])+
		  m[3][0]*(m[1][2]*m[2][1] - m[1][1]*m[2][2]);
	tmp[3][1]=m[0][0]*(m[2][1]*m[3][2] - m[2][2]*m[3][1])+
		  m[2][0]*(m[0][2]*m[3][1] - m[0][1]*m[3][2])+
		  m[3][0]*(m[0][1]*m[2][2] - m[0][2]*m[2][1]);
	tmp[3][2]=m[0][0]*(m[1][2]*m[3][1] - m[1][1]*m[3][2])+
		  m[1][0]*(m[0][1]*m[3][2] - m[0][2]*m[3][1])+
		  m[3][0]*(m[0][2]*m[1][1] - m[0][1]*m[1][2]);
	tmp[3][3]=m[0][0]*(m[1][1]*m[2][2] - m[1][2]*m[2][1])+
		  m[1][0]*(m[0][2]*m[2][1] - m[0][1]*m[2][2])+
		  m[2][0]*(m[0][1]*m[1][2] - m[0][2]*m[1][1]);
	memmove(m, tmp, 4*4*sizeof(double));
}

void
invertm3(Matrix3 m)
{
	Matrix3 m², m³, r;
	double det, trm, trm², trm³;

	det = detm3(m);
	if(det == 0)
		return; /* singular matrices are not invertible */
	trm = tracem3(m);
	memmove(m³, m, 4*4*sizeof(double));
	mulm3(m³, m³);
	mulm3(m³, m);
	trm³ = tracem3(m³);
	memmove(m², m, 4*4*sizeof(double));
	mulm3(m², m²);
	trm² = tracem3(m²);
	identity3(r);
	smulm3(r, (trm*trm*trm - 3*trm*trm² + 2*trm³)/6);
	smulm3(m, (trm*trm - trm²)/2);
	smulm3(m², trm);
	subm3(r, m);
	addm3(r, m²);
	subm3(r, m³);
	smulm3(r, 1/det);
	memmove(m, r, 4*4*sizeof(double));
}

void
cinvertm3(Matrix3 m)
{
	double det;

	det = detm3(m);
	if(det == 0)
		return; /* singular matrices are not invertible */
	adjm3(m);
	smulm3(m, 1/det);
}

void
printm(Matrix m)
{
	int i, j;

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			print("\t%g", m[i][j]);
		print("\n");
	}
}

void
printm3(Matrix3 m)
{
	int i, j;

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++)
			print("\t%g", m[i][j]);
		print("\n");
	}
}

vlong t0, t;

void
main()
{
	Matrix m = {
	//	 7, 2,  1,
	//	 0, 3, -1,
	//	-3, 4, -2
	/* near-singular */
		1, 1, 1,
		0, 1, 0,
		1, 0, 1.01
	}, invm, cinvm;
	Matrix3 M = {
		 1,  1,  1, -1,
		 1,  1, -1,  1,
		 1, -1,  1,  1,
		-1,  1,  1,  1
	}, invM, cinvM;

	memmove(invm, m, 3*3*sizeof(double));
	memmove(cinvm, m, 3*3*sizeof(double));
	print("M:\n");
	printm(m);
	t0 = nsec();
	invertm(invm);
	t = nsec()-t0;
	print("M⁻¹(%lldns):\n", t);
	printm(invm);
	t0 = nsec();
	cinvertm(cinvm);
	t = nsec()-t0;
	print("CM⁻¹(%lldns):\n", t);
	printm(cinvm);
	mulm(m, invm);
	print("MM⁻¹:\n");
	printm(m);
	memmove(invM, M, 4*4*sizeof(double));
	memmove(cinvM, M, 4*4*sizeof(double));
	print("M:\n");
	printm3(M);
	t0 = nsec();
	invertm3(invM);
	t = nsec()-t0;
	print("M⁻¹(%lldns):\n", t);
	printm3(invM);
	t0 = nsec();
	cinvertm3(cinvM);
	t = nsec()-t0;
	print("CM⁻¹(%lldns):\n", t);
	printm3(cinvM);
	mulm3(M, invM);
	print("MM⁻¹:\n");
	printm3(M);
	exits(nil);
}
