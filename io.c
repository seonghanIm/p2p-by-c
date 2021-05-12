#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int fd = open(tempnam("/tmp", "os2021.txt"), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); 
    //tempnam 함수는 프로그램이 실행되는동안 임시로 파일을 만들고 사용하다가 프로그램이 종료되기전에 삭제된다.
    // 반대로 open("/tmp/file")는  tmp/file 이라는 파일을 생성한다.
    assert(fd > -1); 
    int rc = write(fd, "hello world\n", 13); //앞서 만든 임시파일에 hello world 저장
    assert(rc == 13);

    close(fd); // 파일 닫기
    return 0;
}