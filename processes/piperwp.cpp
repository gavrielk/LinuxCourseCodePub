#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <filesystem>
#include <string.h>

const int READ_END = 0;
const int WRITE_END = 1;

const typedef enum {
  TEXT_OP_CODE = 0,
  END_OP_CODE = 1
} TLV_TYPE;

class TLV
{
public:
    int type;
    int length;
    char* value;
    
    TLV(){
        this->type = 0;
        this->length = 0;
        this->value = nullptr;
    }
    
    TLV(int type, int length, const char* value){
        this->type = type;
        this->length = length;
        this->value = new char[length + 1]{0};
        strncpy(this->value, value, length + 1);
    }

    ~TLV(){
      delete(this->value);
    }
};

// Deserialize
TLV* readTlvFromPipe(int pipeReadEnd){
    int len = 0;
    TLV* tlv = new TLV();

    len = read(pipeReadEnd, (void*)&tlv->type , sizeof(tlv->type));
    if (len < 0) { perror("type"); delete(tlv); return nullptr; }

    len = read(pipeReadEnd, (void*)&tlv->length, sizeof(tlv->length));
    if (len < 0 || len > sizeof(tlv->value)) { perror("length"); delete(tlv); return nullptr; }

    if (tlv->length > 0){
      tlv->value = new char[tlv->length]{0};
      len = read(pipeReadEnd, (void*)tlv->value, tlv->length);
      if (len < 0) { perror("value"); delete(tlv); return nullptr; }
    }
    return tlv;
}

// Serialize
void writeTlvToPipe(int pipeWriteEnd, TLV* tlv){
    int len = 0;

    len = write(pipeWriteEnd, (void*)tlv, sizeof(tlv->type) + sizeof(tlv->length));
    if (len < 0) { perror("type + length"); return; }

    len = write(pipeWriteEnd, (void*)tlv->value, tlv->length);
    if (len < 0) { perror("value"); return; }\
}

int main()
{
    char buffer[1024] = {0};
    int pipefd[2] = {0, 0};
    int rc = pipe(pipefd);
    if(rc < 0)
    {
      printf("runtime error: %s", strerror(errno));
      return 0;
    }

    printf("PID: %d\n", getpid());
    printf("Pipe read end: %d\n", pipefd[READ_END]);
    printf("Pipe write end: %d\n", pipefd[WRITE_END]);
    int pid = fork();
    if(pid > 0) // PARENT
    {
        bool endOpCode = false;
        while(endOpCode == false){
          TLV* tlv = readTlvFromPipe(pipefd[READ_END]);
          
          switch (tlv->type)
          {
          case TEXT_OP_CODE:
              printf("Type: %d\n", tlv->type);
              printf("Length: %d\n", tlv->length);
              printf("Value: %s\n", tlv->value);
            break;
          case END_OP_CODE:
              printf("Exiting\n");
              exit(0);
          default:
            break;
          }

          delete(tlv);
        }
      }
    else if(pid == 0) // CHILD
    {
        TLV tlv = {TEXT_OP_CODE, 5, "hello"};
        writeTlvToPipe(pipefd[WRITE_END], &tlv);

        TLV tlv2 = {TEXT_OP_CODE, 5, "world"};
        writeTlvToPipe(pipefd[WRITE_END], &tlv2);

        TLV tlv3 = {END_OP_CODE, 0, "\0"};
        writeTlvToPipe(pipefd[WRITE_END], &tlv3);
    }

    // sleep(10000);

    return 0;
}