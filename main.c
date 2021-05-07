#define _CRT_SECURE_NO_WARNINGS // scanf 보안 경고로 인한 컴파일 에러 방지

#define HAVE_BEEN 2
#define CROSS_ROAD 3
#define CROSS_ROAD_NUM 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int how_many_times;                         //미로개수
int size_x, size_y;                         //미로의 크기 (x,y)
int start_X=0, start_Y=0;                   //x,y 초기화 시 사용

int Find_Path(int x,int y, int **map,int *result, int *cr_num);            //경로의 개수를 세는 Recursive 함수
int cross_road_counter(int x, int y,int *cr_num);                          //갈림길이 나오면 좌표를 cross_road xy[]에 입력해주는 함수
int result_memory(int result_num,int *result,int result_arr[]);            //결과값 저장 함수
int return_result(int result_arr[],int how_many);                          //결과값 반환 함수
void free_Map(int **map);                                                  //미로 데이터 동적할당을 해제시키는 함수

typedef struct Cross_Road_{                                                //갈림길 좌표 구조체
    int cross_road_x;
    int cross_road_y;
}cross_road;

cross_road cr_xy[CROSS_ROAD_NUM];                                          //갈림길 좌표 실제로 저장 할 변수



int Find_Path(int x,int y, int **map, int *result, int *cr_num)
{
    if((*result)>=1) return 0;
    if(x<0||y<0||x>size_x-1||y>size_y-1)    //맵을 벗어날 시 1 반환
    {
        return 1;
    }
    if(x==size_x-1&&y==size_y-1)            //골인 지점 도달 시 result++
    {
        (*result)++;
        return 0;
    }
    if(y>0&&map[y-1][x]==1)                                             //위로 이동가능
    {
        if((x<size_x-1&&map[y][x+1]==1)||(y<size_y-1&&map[y+1][x]==1))  //위로 이동가능, 갈림길
        {
            cross_road_counter(x, y, cr_num);
            map[y][x]=CROSS_ROAD;
            
            return Find_Path(x, y-1, map,result,cr_num);
        }
        else                                                            //위로 이동가능, 외길
        {
            map[y][x]=HAVE_BEEN;
            return Find_Path(x, y-1, map, result,cr_num);
        }
    }
    else if(x<size_x-1&&map[y][x+1]==1)     //위로 이동불가, 오른쪽 이동가능
    {
        if(y<size_y-1&&map[y+1][x]==1)      //위로 이동불가, 오른쪽 이동가능, 갈림길
        {
            cross_road_counter(x, y, cr_num);
            map[y][x]=CROSS_ROAD;
            return Find_Path(x+1, y, map, result,cr_num);
        }
        else                                //위로 이동불가, 오른쪽 이동가능, 외길
        {
            map[y][x]=HAVE_BEEN;
            return Find_Path(x+1, y, map, result,cr_num);
        }
    }
    else if(y<size_y-1&&map[y+1][x]==1)     //위로 이동불가, 오른쪽 이동불가, 아래 외길
    {
        map[y][x]=HAVE_BEEN;
        return Find_Path(x, y+1, map, result,cr_num);
    }
    else                                    //이동불가
    {
        map[y][x]=HAVE_BEEN;
        if((*cr_num)==0) return 0;          //이동불가, 저장된 갈림길좌표 없을 시 return 0;
        if((*cr_num)>0)                     //이동불가, 저장된 갈림길좌표 있을 시 갈림길 좌표로 Recursive
        {
            (*cr_num)--;
            return Find_Path(cr_xy[*cr_num].cross_road_x, cr_xy[*cr_num].cross_road_y, map, result,cr_num);
        }
        else return 1;                      //이외 에러 발생 시 return 1
    }
}

int cross_road_counter(int x, int y, int *cr_num)   //갈림길 개수 저장 함수
{
    cr_xy[*cr_num].cross_road_x=x;
    cr_xy[*cr_num].cross_road_y=y;
    (*cr_num)++;
    return 0;
}
int result_memory(int result_num,int *result,int result_arr[])      //결과값 저장 함수
{
    result_arr[result_num]=(*result);
    return 0;
}
int return_result(int result_arr[],int how_many)                    //결과값 반환 함수
{
    FILE *of = fopen("output.txt","w");
    for(int i=0;i<how_many;i++)
    {
        if(result_arr[i]==0)                                        //결과가 0이면 0 파일출력
        {
            fprintf(of, "%d\n",result_arr[i]);
        }
        else if(result_arr[i]>0)                                    //결과가 0이 아니면 1 출력
        {
            fprintf(of, "%d\n",1);
        }
        else return 0;
    }
    fclose(of);
    return 0;
}

void free_Map(int **map)                                            //미로 데이터 동적할당 해제 함수
{
    for(int i=0;i<size_y;i++)
    {
        free(map[i]);
    }
    free(map);
}


int main()
{
    FILE *fp = fopen("input.txt","r");
        fscanf(fp, "%d", &how_many_times);                      //미로의 개수 입력
    int *result_arr=(int*)malloc(sizeof(int)*how_many_times);   //미로 개수만큼 결과값 할당
        for(int i=0;i<how_many_times;i++)                       //미로의 개수만큼 반복
        {
            for(int i=0;i<CROSS_ROAD_NUM;i++)                   //저장된 이전 맵 갈림길 좌표 리셋
            {
                memset(&cr_xy[i].cross_road_x,0,sizeof(int));
                memset(&cr_xy[i].cross_road_y,0,sizeof(int));
            }
            int result=0;                                           //새로운 맵생성 전 결과값 초기화
            int cross_road_num=0;                                   //새로운 맵생성 전 갈림길 개수 초기화
            
            fscanf(fp, "%d %d", &size_y, &size_x);              //미로 크기 input
            int **m = (int**)malloc(sizeof(int *) * size_y);    //배열의 세로 크기만큼 이중 포인터에 동적 메모리 할당.

            for (int i = 0; i < size_y; i++)                    // 세로 크기만큼 반복
            {
                m[i] = (int*)malloc(sizeof(int) * size_x);      // (int 크기 * 가로 크기)만큼 동적 메모리 할당. 배열의 가로
            }
            for(int i=0;i<size_y;i++)                           //미로 데이터를 input
            {
                for(int j=0;j<size_x;j++)
                {
                    fscanf(fp, "%d", &m[i][j]);
                }
            }
            
            Find_Path(start_X, start_Y,m, &result, &cross_road_num);              //탈출 가능성 파악, recursive 사용

            result_memory(i,&result,result_arr);                                  //결과값 배열에 저장
            
            free_Map(m);                                                          //동적할당 해제, 미로 리셋
        }
    return_result(result_arr,how_many_times);                                 //결과값 배열 output 파일에 반환
    fclose(fp);                                                               //파일 닫음
    free(result_arr);                                                         //결과값 배열 해제
    return 0;
}
