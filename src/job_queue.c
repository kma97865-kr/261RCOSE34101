#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "queue.h"
#include "job_queue.h"

/*struct _queue{ 
    struct node *front; //points to first node
    struct node *rear; //points to last node
};*/
//구조체에 직접 접근 불가 ->Getter, Setter 함수 정의해서 써야 한다.

int compare_arrival_time(const void *a, const void *b) { //응용 -> priority 기반 정렬도 가능은 하다
    Process p1 = queue_node_get_data(*(Node *) a);
    Process p2 = queue_node_get_data(*(Node *) b);
    //int - int 는 overflow 날 수 있는 곳
    return (int)process_arrival_time(p1) - (int)process_arrival_time(p2);
    
}

//arrival time 대로 정렬->qsort함수 사용
void job_queue_sort(void){
    //job_queue에 있는 프로세스 수 만큼 Process 동적 배열을 하나 만들고
    int count = queue_count(job_queue);
    Node *nodearr = malloc(sizeof(Node) * count);
    if (nodearr == NULL) {
        printf("동적 배열 메모리 할당 실패");
        exit(EXIT_FAILURE);
    }

    Node temp = queue_front_node(job_queue);
    for(int i = 0; i<count; i++){
        printf("i is %d\n", i);
        nodearr[i] = temp;
        temp = queue_node_next_node(temp);
    }
    //개네를 qsort 시킨 다음
    /*void qsort(
    void *base,   // 1. 정렬할 배열의 시작 주소
    size_t nmemb, // 2. 배열의 데이터 개수
    size_t size,  // 3. 데이터 1개의 크기 (sizeof 사용)
    int (*compar)(const void *, const void *) // 4. 두 값을 비교하는 커스텀 함수
    );*/
    qsort(nodearr, count, sizeof(Node), compare_arrival_time);

    for(int i = 0; i<count; i++){
        process_show(queue_node_get_data(nodearr[i]));
        printf("\n");
    }

    //다시 연결 리스트로 만들어 준다.
    //job_queue를 부수고 다시 만들 필요 없이, next만 잘 이어주면 된다
    for (int i = 0; i < count - 1; i++) {
        queue_node_set_next(nodearr[i], nodearr[i+1]);
    }
    // 가장 마지막 노드는 꼬리를 잘라줌
    queue_node_set_next(nodearr[count - 1], NULL);

    //큐의 머리와 꼬리를 새로운 노드로 교체
    queue_set_front(job_queue, nodearr[0]);
    queue_set_rear(job_queue, nodearr[count - 1]);

    // (디버깅) 결과 출력
    printf("here is sorted job_queue\n");
    for(int i = 0; i < count; i++) {
        process_show(queue_node_get_data(nodearr[i]));
    }

    free(nodearr);
}

//time 시간을 받고, 큐를 돌면서 : arrival time이 된 애들이 여러 애들이면? -> NULL 나올 때까지 pop
//(여기 책임 x)
//가정 : job_queue 는 arrival time에 대해 정렬되어 있음
Process job_queue_to_ready(int time){
    if(queue_is_empty(job_queue)) return NULL;

    Process temp = queue_front(job_queue);
    if(process_arrival_time(temp) <= (unsigned int)time){
        return queue_pop(job_queue);
    }else{
        return NULL;
    }
}