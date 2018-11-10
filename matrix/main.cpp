#include<stdio.h>
#include<stdlib.h>
#include <math.h>

//����һ��bool���ͣ�C������Ҫ��
typedef enum { False = 0, True = 1 }Bool;

//����һ���ṹ�壬������ʾһ����ά�ľ���
typedef struct Matrix
{
	int row;
	int column;
	double *data;//������ž����Ԫ��
};

/************************************************************************
�������ܣ���ʼ��һ������
���룺Ҫ��ʼ���ľ���matrix���������row���������column
�������ʼ���ɹ���True����ʼ��ʧ�ܣ�False
************************************************************************/
Bool InitMatrix(Matrix *matrix, int row, int column)
{
	int matrix_size = row*column*sizeof(double);
	if (matrix_size <= 0)
		return False;
	matrix->data = (double*)malloc(matrix_size);//���������ռ�
	if (matrix->data)
	{
		matrix->row = row;
		matrix->column = column;
		return True;
	}
	else
	{
		matrix->row = 0;
		matrix->column = 0;
		return False;
	}
}

/************************************************************************
�������ܣ���ӡ��һ������
���룺һ������matrix
�������
************************************************************************/
void PrintMatrix(Matrix *matrix)
{
	int matrix_num = matrix->row*matrix->column;
	for (int i = 0; i < matrix_num; i++)
	{
		printf("%12.4g  ", matrix->data[i]);
		if ((i + 1) % (matrix->column) == 0)
			printf("\n");
	}
	printf("\n");
}

/************************************************************************
�������ܣ���ȡһ������Ĵ�С
���룺һ������matrix
���������Ĵ�Сsize
************************************************************************/
int GetMatrixSize(Matrix *matrix)
{
	return matrix->row*matrix->column;
}

/************************************************************************
�������ܣ����㣬ʹ����ÿ��Ԫ��Ϊ0
���룺��Ҫ����ľ���matrix
�������
************************************************************************/
void SetMatrixZeros(Matrix *matrix)
{
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		matrix->data[i] = 0;
}

/************************************************************************
�������ܣ��ж�һ�������Ƿ�Ϊ��
���룺һ������matrix
�����Ϊ����True������ΪFalse
************************************************************************/
Bool IsNullMatrix(Matrix *matrix)
{
	int matrix_num =GetMatrixSize(matrix);
	if ((matrix_num <= 0) || (matrix->data == NULL))
		return True;
	else
		return False;
}

/************************************************************************
�������ܣ��ͷŵ�һ������
���룺һ������matrix
�������
************************************************************************/
void DestroyMatrix(Matrix *matrix)
{
	if (!IsNullMatrix(matrix))
	{
		matrix->data = NULL;
		matrix->row = 0;
		matrix->column = 0;
		free(matrix->data);
	}
}

/************************************************************************
�������ܣ�����һ�������2����������ģ
���룺һ������matrix
���������ķ������norm2_ans
************************************************************************/
double MatrixNorm2(Matrix *matrix)
{
	double norm2_ans = 0.0;
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		norm2_ans+=(matrix->data[i]) * (matrix->data[i]);
	norm2_ans = (double)sqrt(norm2_ans);
	return norm2_ans;
}

/************************************************************************
�������ܣ���һ��������
���룺��Ҫ���и��Ƶľ���matrix_A�����Ƶõ���һ������matrix_B
�������
************************************************************************/
void CopyMatrix(Matrix *matrix_A, Matrix *matrix_B)
{
	if (matrix_B->row != matrix_A->row)
		matrix_B->row = matrix_A->row;
	if (matrix_B->column != matrix_A->column)
		matrix_B->column = matrix_A->column;
	int size_A = GetMatrixSize(matrix_A);
	for (int i = 0; i < size_A; i++)
		matrix_B->data[i] = matrix_A->data[i];
}

/************************************************************************
�������ܣ���һ������A����QR�ֽ�
���룺��Ҫ�ֽ�ľ���A���ֽ�����������Q�������Ǿ���R
�������
************************************************************************/
void QR(Matrix *A, Matrix *Q, Matrix *R)
{
	Matrix col_A, col_Q;
	InitMatrix(&col_A, A->row, 1);
	SetMatrixZeros(&col_A); //������A��ÿһ��
	InitMatrix(&col_Q, A->row, 1);
	SetMatrixZeros(&col_Q);  //������Q��ÿһ��

	if (A->row != A->column)
		printf("A is not a square matrix!");

	int A_size = GetMatrixSize(A);
	int Q_size = GetMatrixSize(Q);
	int R_size = GetMatrixSize(R);

	if (Q_size != A_size)
	{
		DestroyMatrix(Q);
		InitMatrix(Q, A->row, A->column);
		SetMatrixZeros(Q);
	}
	else
	{
		Q->row = A->row;
		Q->column = A->column;
		SetMatrixZeros(Q);
	}
	
	if (R_size != A_size)
	{
		DestroyMatrix(R);
		InitMatrix(R, A->row, A->column);
		SetMatrixZeros(R);
	}
	else
	{
		R->row = A->row;
		R->column = R->column;
		SetMatrixZeros(R);
	}

	//ʩ����������
	for (int j = 0; j < A->column; j++)
	{
		for (int i = 0; i < A->column; i++)//��A�ĵ�j�д���col_A��
		{
			col_A.data[i] = A->data[i * A->column + j];
			col_Q.data[i] = A->data[i * A->column + j];
		}
		for (int k = 0; k < j; k++)//�����j����ǰ
		{
			R->data[k * R->column + j] = 0;
			for (int i1 = 0; i1 < col_A.row; i1++)
			{//R=Q'A(Q'��Q��ת��) ��Q�ĵ�k�к�A�ĵ�j�����ڻ�
				R->data[k * R->column + j] += col_A.data[i1] * Q->data[i1 * Q->column + k];//Q�ĵ�k��
			}
			for (int i2 = 0; i2 < A->column; i2++)
			{
				col_Q.data[i2] -= R->data[k * R->column + j] * Q->data[i2 * Q->column + k];
			}
		}
		
		double temp = MatrixNorm2(&col_Q);
		R->data[j * R->column + j] = temp;
		for (int i3 = 0; i3 < Q->column; i3++)
		{
			//��λ��Q
			Q->data[i3 * Q->column + j] = col_Q.data[i3] / temp;
		}
	}

	DestroyMatrix(&col_A);
	DestroyMatrix(&col_Q);
}

/************************************************************************
�������ܣ�������ֵ���򣬵�flag=1ʱ�������򣬵�flag=0������
���룺��Ҫ���������eValue�������ǽ����ѡ��flag
���������ɹ��򷵻�True�����򷵻�False
************************************************************************/
Bool SortEigenValues(Matrix *eValue, int flag)
{
	int size = GetMatrixSize(eValue);

	for (int i = 0; i < size - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < size; j++)
		{
			if (flag == 1)
			{
				if (eValue->data[k] > eValue->data[j])
				{
					k = j;
				}
			}
			else if (flag == 0)
			{
				if (eValue->data[k] < eValue->data[j])
				{
					k = j;
				}
			}
			else
				return False;
		}
		if (k != i)
		{
			double temp;
			temp = eValue->data[i];
			eValue->data[i] = eValue->data[k];
			eValue->data[k] = temp;
		}
	}
	return True;
}

/************************************************************************
�������ܣ����������������C=A*B
���룺������������ľ���C����Ҫ���г˷��������������A��B
���������ɹ������True��ʧ����False
************************************************************************/
Bool MatrixMulMatrix(Matrix *C, Matrix *A, Matrix *B)
{
	if ((IsNullMatrix(A)) || (IsNullMatrix(B)))
		return False;

	int A_col = A->column;
	int B_row = B->row;
	InitMatrix(C, A->row, B->column);
	SetMatrixZeros(C);

	if (A_col != B_row)
	{
		printf("A_col!=B_row!");
		return False;
	}

	for (int i = 0; i < A->row; i++)
	{
		for (int j = 0; j < B->column; j++)
		{
			for (int k = 0; k < A->column; k++)
				C->data[i*C->row + j] += A->data[i*A->row + k] * B->data[k*B->column + j];
		}
	}

	return True;
}



/************************************************************************
�������ܣ���֪һ�����������ֵ���Ӧ����������
���룺һ������A��������������������eigenVector����֪������ֵeigenValue
�������
************************************************************************/
void Eig(Matrix *A, Matrix *eigenVector, Matrix *eigenValue)
{
	int num = A->column;
	double eValue;
	Matrix temp;
	InitMatrix(&temp, A->row, A->column);
	//CopyMatrix(A, &temp);
	for (int count = 0; count < num; count++)
	{
		eValue = eigenValue->data[count];//��ǰ������ֵ
		CopyMatrix(A, &temp);//���ÿ�ζ�Ҫ���¸��ƣ���Ϊ������ƻ�ԭ����(�տ�ʼûע�⵽�����bug���˺þá���)
		for (int i = 0; i < temp.row; i++)
		{
			temp.data[i * temp.column + i] -= eValue;
		}
		//��temp��Ϊ�����;���(��һ��)�Խ���ֵΪһ
		for (int i = 0; i < temp.row - 1; i++)
		{
			double coe = temp.data[i * temp.column + i];
			for (int j = i; j < temp.column; j++)
			{
				temp.data[i * temp.column + j] /= coe;//�öԽ���ֵΪһ
			}
			for (int i1 = i + 1; i1 < temp.row; i1++)
			{
				coe = temp.data[i1 * temp.column + i];
				for (int j1 = i; j1 < temp.column; j1++)
				{
					temp.data[i1 * temp.column + j1] -= coe * temp.data[i * temp.column + j1];
				}
			}
		}
		//�����һ��Ϊ1
		double sum1 = eigenVector->data[(eigenVector->row - 1) * eigenVector->column + count] = 1;
		for (int i2 = temp.row - 2; i2 >= 0; i2--)
		{
			double sum2 = 0;
			for (int j2 = i2 + 1; j2 < temp.column; j2++)
			{
				sum2 += temp.data[i2 * temp.column + j2] * eigenVector->data[j2 * eigenVector->column + count];
			}
			sum2 = -sum2 / temp.data[i2 * temp.column + i2];
			sum1 += sum2 * sum2;
			eigenVector->data[i2 * eigenVector->column + count] = sum2;
		}
		sum1 = sqrt(sum1);//��ǰ�е�ģ
		for (int i = 0; i < eigenVector->row; i++)
		{
			//��λ��
			eigenVector->data[i * eigenVector->column + count] /= sum1;
		}
	}
	DestroyMatrix(&temp);
}

int main()
{
	const unsigned NUM = 50; //�����������������ݸ�׼ȷ
	Matrix mymatrix,temp,temp_Q,temp_R, eValue;
	int row,col;
	while (1)//VS��������дscanf�ᱨ���ĳ�scanf_s�Ϳ�����
	{
		printf("������Ҫ���м���ľ���������(�Զ��Ÿ���)��");
		scanf_s("%d,%d", &row, &col);

		InitMatrix(&mymatrix, row, col);
		InitMatrix(&temp, row, col);
		SetMatrixZeros(&temp);
		SetMatrixZeros(&mymatrix);

		int num = row*col;
		printf("�������е�����˳��������������ڵ�Ԫ�أ�һ������%d��Ԫ�أ�", num);
		int data;
		for (int i = 0; i < num; i++)
		{
			scanf_s("%d", &data);
			mymatrix.data[i] = data;
		}
		printf("����������£�\n");
		PrintMatrix(&mymatrix);

		CopyMatrix(&mymatrix, &temp);

		InitMatrix(&temp_Q, mymatrix.row, mymatrix.column);
		InitMatrix(&temp_R, mymatrix.row, mymatrix.column);
		InitMatrix(&eValue, mymatrix.row, 1);

		//ʹ��QR�ֽ����������ֵ
		for (int k = 0; k < NUM; ++k)
		{
			QR(&temp, &temp_Q, &temp_R);
			MatrixMulMatrix(&temp, &temp_R, &temp_Q);//R*Q
		}

		/*printf("Q&R:\n");
		PrintMatrix(&temp_Q);
		PrintMatrix(&temp_R);*/

		//��ȡ����ֵ����֮�洢��eValue
		for (int k = 0; k < temp.column; ++k)
		{
			eValue.data[k] = temp.data[k * temp.column + k];
		}

		SortEigenValues(&eValue, 1);//������ֵ����1Ϊ����0Ϊ����
		printf("����ֵ��\n");
		PrintMatrix(&eValue);

		Eig(&mymatrix, &temp_Q, &eValue);
		printf("����������\n");
		PrintMatrix(&temp_Q);

		DestroyMatrix(&eValue);
		DestroyMatrix(&mymatrix);
		DestroyMatrix(&temp);
		DestroyMatrix(&temp_Q);
		DestroyMatrix(&temp_R);
	}
	return 0;
}