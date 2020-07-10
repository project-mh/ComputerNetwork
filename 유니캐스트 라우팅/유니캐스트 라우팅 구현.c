
/////////////////////////////// 매크로 상수 & 전역 변수 & 헤더 파일 ////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS 
#define NO_CONNECTION 19961204					// 연결이 없는 경우 (큰 정수)
#define PEAK_NUMBER 6							// 정점의 수

int DUPLICATE_ROUTE[PEAK_NUMBER][PEAK_NUMBER];	// 중복 경로 저장
int Matrix_path[PEAK_NUMBER][PEAK_NUMBER];		// 최단거리 정점까지 거치는 노드들을 저장
int COST[PEAK_NUMBER][PEAK_NUMBER];				// 입력받을 행렬
int DISTANCE[PEAK_NUMBER];						// 시작 정점으로부터의 최단 경로 거리
int FOUND[PEAK_NUMBER];							// 방문한 정점 표시

#include <stdio.h>
#include <stdlib.h>
/////////////////////////////// 매크로 상수 & 전역 변수 & 헤더 파일 ////////////////////////////////////////

// (10)
void
Print_Route(int start) {
	/////////////////////////////// 출력 ////////////////////////////
	// 저장된 경로들을 (A,0,B,C) (B,2,C,E) ... 와 같이 출력합니다
	// 1. 시작 정점과 시작 정점에서의 각각의 거리를 출력합니다.
	// 2. DUPLICATE_ROUTE은 최종적으로 한 정점에서 중복된 경로가 삭제되어 있고,
	// 경로가 0번째 인덱스부터 저장되어 있으므로, 
	// 0번째 인덱스부터 출력을 하되, 인덱스가 연결되지 않은 경우가 나오면 출력하지 않습니다.

	printf(" > Least-cost tree rooted at %c : ", start + 65);
	for (int i = 0; i < PEAK_NUMBER; i++) {
		printf("(%c, %d", i + 65, DISTANCE[i]);

		for (int j = 0; j < PEAK_NUMBER; j++) {
			if (DUPLICATE_ROUTE[i][0] == NO_CONNECTION) {
				printf(", NULL");
				break;
			}
			if (DUPLICATE_ROUTE[i][j] != NO_CONNECTION)
				printf(", %c", DUPLICATE_ROUTE[i][j] + 65);
		}
		printf(")");
		if (i != PEAK_NUMBER - 1)
			printf(", ");
	}

	printf("\n\n");
	///////////////////////////////출력 끝////////////////////////////
}

// (9)
void
Delete_Duplicate_Route() {
	///////////////////// 중복 경로 삭제 /////////////////////////
	// 1. DUPLICATE_ROUTE의 첫번째 열의 원소들을 나머지 열들의 원소들과 비교해서
	// 같은 경로가 있다면 나머지 열에 저장된 경로를 연결이 없음으로 바꿔줍니다.
	// 2. 첫번째 열에서 중복되는 경로를 NO_CONNECTION 제외하고 찾습니다.
	// DUPLICATE_ROUTE에는 중복되는 경로의 열이 저장되어 있습니다.
	// Matrix_path에는 최단거리 경로 거리가 저장되어 있습니다.
	// 같은 열에서 NO_CONNECTION를 제외하고 중복된 경로를 찾으면
	// 그 열의 다음 원소를 그 앞의 원소로 옮겨줍니다. (2 -> 1, 3 -> 2, 4 -> 3, ...)

	// 중복 경로 삭제하기위한 변수,,,
	int delete_route = 0;
	
	for (int one = 0; one < PEAK_NUMBER; one++)
		for (int two = 0; two < PEAK_NUMBER; two++) {
			for (int thr = 1; thr < PEAK_NUMBER; thr++) {
				for (int fou = 0; fou < PEAK_NUMBER; fou++) {
					if (((two == fou) && (one == thr)))
						break;
					if (DUPLICATE_ROUTE[two][one] == DUPLICATE_ROUTE[fou][thr]) {
						DUPLICATE_ROUTE[fou][thr] = NO_CONNECTION;
					}
				}
			}
		}


	for (int one = 0; one < PEAK_NUMBER; one++)
		for (int two = 0; two < PEAK_NUMBER; two++)
			for (int thr = two + 1; thr < PEAK_NUMBER; thr++)
				if ((DUPLICATE_ROUTE[two][one] == DUPLICATE_ROUTE[thr][0])) {
					if (DUPLICATE_ROUTE[two][one] != NO_CONNECTION) {
						delete_route = DUPLICATE_ROUTE[two][one];
						if (Matrix_path[two][delete_route] < Matrix_path[thr][delete_route])
							for (int fou = one; fou < PEAK_NUMBER - 1; fou++)
								DUPLICATE_ROUTE[thr][fou] = DUPLICATE_ROUTE[thr][fou + 1];
						else
							for (int fiv = one; fiv < PEAK_NUMBER - 1; fiv++)
								DUPLICATE_ROUTE[two][fiv] = DUPLICATE_ROUTE[two][fiv + 1];
					}
				}
	///////////////////// 중복 경로 삭제 끝 /////////////////////////
}

// (8)
int
Find_Matrix_Vertex() {
	//////////////////// 최단거리에 있는 정점 찾기 //////////////////	
	// 1. 현재 DISTANCE[]에서 가장 작은 가중치 값이 위치하고 있는 배열의 인덱스를 찾아 반환합니다.
	// 2. 최소값 min을 찾아 방문하지 않은 정점에 대해 DISTANCE[] 값을 비교합니다.
	// 방문한 적 없는 정점이고 현재까지의 최소값보다 작으면 최소값을 갱신하고
	// 최소값이 등장했으므로 mp에 정점의 인덱스를 저장합니다
	// 3. 최소값을 반환합니다.

	int min = NO_CONNECTION;
	int mp = -1;

	for (int minima = 0; minima < PEAK_NUMBER; minima++)
		if (DISTANCE[minima] < min && !FOUND[minima]) {
			min = DISTANCE[minima];
			mp = minima;
		}

	return mp;
	//////////////////// 최단거리에 있는 정점 찾기 끝 //////////////////
}

// (7)
void
Route_Storage(int start) {
	//////////////////// 중복 경로 저장하기 //////////////////
	// 1. 시작하는 정점은 방문했음(TRUE)으로 표시합니다.	
	// 2. 시작 지점을 지정해줍니다.
	// 3. 중복 경로를 저장합니다. 
	// 마지막에 경로를 출력할 때 배열을 사용해서 출력하므로,
	// 경로를 0부터 차례대로 저장해야합니다.
	// 아직 방문하지 않은 정점들 중에서
	// txt 파일로부터 받은 Matrix가 저장된 COST의 값들을 비교해서 연결된 부분을 찾고,
	// 현재 그 정점까지의 거리 + 다음 정점까지 거리가 
	// 기존 정점까지 거리보다 같거나 가까우면 DUPLICATE_ROUTE에 인덱스를 저장합니다.
	// DUPLICATE_ROUTE는 현재 노드에서 갈 수 있는 정점을 출력할 때 사용합니다.
	// 4. Find_Matrix_Vertex() 함수를 호출해 최소값이 있는 인덱스를 찾고 그 인덱스에 방문표시를 합니다.
	// 5. 아직 방문하지 않은 정점들 중에서 
	// 현재 그 정점까지의 거리 + 다음 정점까지 거리가 
	// 기존 정점까지 거리보다 가까우면 DISTANCE와 Matrix_path를 갱신합니다.

	FOUND[start] = 1;
	DISTANCE[start] = 0;

	// 입력을 A로 했을때 >>> u = A 입니다.
	//		  B			 >>> u = B 입니다.
	int Display = start;

	// 총 정점 개수에서 시작 정점 하나를 뺀만큼 반복해야 합니다.
	for (int i = 0; i < PEAK_NUMBER - 1; i++) {

		// 중복 경로를 저장합니다.
		int val = 0;

		for (int save = 0; save < PEAK_NUMBER; save++)
			if (!FOUND[save] && COST[Display][save] != NO_CONNECTION)
				if (DISTANCE[Display] + COST[Display][save] <= DISTANCE[save]) {
					DUPLICATE_ROUTE[Display][val] = save;
					val++;
				}

		// 방문표시를 합니다.
		Display = Find_Matrix_Vertex();
		FOUND[Display] = 1;

		// 정보를 갱신합니다.
		for (int Re = 0; Re < PEAK_NUMBER; Re++)
			if (!FOUND[Re])
				if (DISTANCE[Display] + COST[Display][Re] < DISTANCE[Re])
				{
					DISTANCE[Re] = DISTANCE[Display] + COST[Display][Re];
					Matrix_path[Display][Re] = DISTANCE[Re];
				}
	}
	//////////////////// 중복 경로 저장하기 끝 //////////////////
}

// (6)
void
Find_Shortest_Path(int start) {
	////////////////////  최단 경로 거리 구하기 //////////////////
	// 1. DISTANCE, FOUND, Matrix_path를 각각 초기화 합니다.
	// DISTANCE는 시작 정점으로부터의 최단 경로 거리이므로 현재 입력받은 정점의 행
	// 즉 A를 입력받았을 때 A에서의 최단 경로 거리는 [ 0, 2, 0, 3, 0, 0, 0 ]가 됩니다.
	// FOUND는 방문한 정점을 표시하므로, 현재 방문한 정점이 없으므로 FALSE입니다.
	// Matrix_path는 최단거리 정점까지 거치는 노드들을 저장하므로 현재 입력받은 정점의
	// 인덱스를 0번째 열에 저장합니다.
	// Matrix_path는 DUPLICATE_ROUTE와 비교하며 중복 경로를 찾습니다.
	// 2. Route_Storage() 함수를 호출하여 중복 경로를 저장합니다.
	// 3. Delete_Duplicate_Route() 함수를 호출하여 중복 경로를 삭제합니다.
	// 4. Print_Route() 호출하여 시작 정점부터 마지막 정점까지 최단 경로 출력합니다.	

	for (int i = 0; i < PEAK_NUMBER; i++) {
		DISTANCE[i] = COST[start][i];
		FOUND[i] = 0;
		Matrix_path[i][0] = start;
	}

	Route_Storage(start);
	Delete_Duplicate_Route();
	Print_Route(start);
	////////////////////  최단 경로 거리 구하기 끝 //////////////////	
}

// (5)
void
Array_init(int arr[][PEAK_NUMBER]) {
	///////////////////// 초기화 /////////////////////////
	// 모든 Matrix의 값을 연결되지 않음으로 바꿔줍니다.
	// 초기화하는 Matrix들은
	// 최단거리를 구할 때, 중복된 경로를 삭제할 때 사용합니다.

	for (int i = 0; i < PEAK_NUMBER; i++)
		for (int j = 0; j < PEAK_NUMBER; j++)
			arr[i][j] = NO_CONNECTION;
	///////////////////// 초기화 끝 /////////////////////////
}

// (4)
void
InputOutput_Running_Test() {
	///////////////////// 프로그램 시작 /////////////////////////	
	// 1. 시작 정점 입력받습니다.
	// 시작 정점을 char로 입력받게 되면, enter까지 버퍼에 저장되기 때문에
	// getchar()을 사용해 \n을 삭제했습니다.	
	// 2. 먼저 Matrix_path와, DUPLICATE_ROUTE를 Array_init() 함수를 호출하여 초기화합니다.
	// Matrix_path와 DUPLICATE_ROUTE는 각각 전역변수로 선언되어 있습니다.	
	// 3. Find_Shortest_Path() 함수를 호출하여 최단 경로를 구합니다.

	char start, START;

	while (1) {
		printf(" > input the source router : ");
		scanf_s("%c", &start, 1);
		START = getchar(start != '\n');

		Array_init(Matrix_path);		Array_init(DUPLICATE_ROUTE);
		Find_Shortest_Path(start - 65);
	}
	///////////////////// 프로그램 끝 /////////////////////////
}

// (3)
void
Read_Cost(int Matrix[][PEAK_NUMBER], FILE* file) {
	//////////////////////// txt파일 변경 //////////////////////////
	// txt파일을 읽어왔습니다.
	// 현재 과제에서 읽어온 Matrix는 다음과 같습니다.
	//	0, 2, 0, 3, 0, 0, 0
	//	2, 0, 5, 0, 4, 0, 0
	//	0, 5, 0, 0, 0, 4, 3
	//	3, 0, 0, 0, 5, 0, 0
	//	0, 4, 0, 5, 0, 2, 0
	//	0, 0, 4, 0, 2, 0, 1
	//	0, 0, 3, 0, 0, 1, 0 
	// 값이 0인 부분은 정점끼리 서로 연결되지 않았습니다.
	// 계산을 편하게 하기 위해 연결되지 않은 성분은
	// 매크로 상수인 NO_CONNECTION로 바꿔줍니다.
	// 매크로 상수 NO_CONNECTION는 큰 정수값을 가지고 있습니다.

	for (int i = 0; i < PEAK_NUMBER; i++)
		for (int j = 0; j < PEAK_NUMBER; j++) {
			int r = fscanf(file, "%d ,", Matrix[i] + j);

			if (r < 1) {
				fprintf(stderr, "자료가 부족합니다.\n");
				exit(-1);
			}
			// 연결되지않은 노드의 값 변경
			if (Matrix[i][j] == 0)
				Matrix[i][j] = NO_CONNECTION;
		}
	//////////////////////// txt파일 변경 끝 //////////////////////////
}

// (2)
void
File_InputOutput() {
	///////////////////// 파일 입출력 /////////////////////////
	// txt파일을 읽어옵니다.

	FILE* fp = fopen("./Matrix_txt_6x6.txt", "r");

	if (fp == NULL) {
		puts("file open failed!!");		return -1;
	}

	Read_Cost(COST, fp);	fclose(fp);
	///////////////////// 파일 입출력 끝 /////////////////////////
}

// (1)
void
Compile_Message() {
	///////////////////// 메세지를 출력합니다 /////////////////////////
	// 메세지와 현재 정점의 갯수를 출력하며, 정점의 개수는 매크로 상수로 정의됩니다.

	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("%%		Program to find the least-cost tree for unicast routing				%%\n");
	printf("%%					least-cost tree's vertex : %d				%%\n", PEAK_NUMBER);
	printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n");
	///////////////////// 메세지를 출력합니다 끝 /////////////////////////
}

// START
int
main() {
	///////////////////// main 함수 /////////////////////////	
	// 1. message를 출력합니다.
	// 2. txt 파일을 읽어옵니다.
	// 3. 읽어온 txt파일로 다익스트라 알고리즘을 사용하여 각각 정점에서의 경로를 구합니다.

	Compile_Message();
	File_InputOutput();
	InputOutput_Running_Test();
	///////////////////// main 함수 끝 /////////////////////////
}

