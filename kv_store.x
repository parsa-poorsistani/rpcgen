typedef char buf <>;

struct kv {
    buf key;
    buf value;
};

 program KVSTORE {
	version KVSTORE_V1 {
		int EXAMPLE(int) = 1;
    buf ECHO(buf) = 2;
    void PUT(kv) = 3;
    buf GET(buf) = 4;
	} = 1;
} = 0x20000001;
