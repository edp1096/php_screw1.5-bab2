// zencode_test_read_vc11.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <string.h>
#include <sys/stat.h>

#ifdef WIN32
#include "php_screw.h"
#include "my_screw.h"
#else
#include "../php_screw.h"
#include "../my_screw.h"
#endif

//int main(int argc, _TCHAR* argv[])
int main(int argc, char* argv[])
{
	FILE *fp;
	char buf[PM9SCREW_LEN + 1];
	struct stat stat_buf;
	char *screw_datap, *descrew_datap;
	int screw_datalen, descrew_datalen;
	int cryptkey_len = sizeof pm9screw_mycryptkey / 2;
	char descrew_filename[256];
	int i;

	// 암호화데이터 준비
	if (argc != 2) {
		fprintf(stderr, "Usage: filename.\n");
		exit(0);
	}

	fp = fopen((char *)argv[1], "rb");
	if (fp == NULL) {
		fprintf(stderr, "File not found(%s)\n", argv[1]);
		exit(0);
	}

	fread(buf, PM9SCREW_LEN, 1, fp);
	if (memcmp(buf, PM9SCREW, PM9SCREW_LEN) != 0) {
		fprintf(stderr, "Not a crypted file.\n");
		fclose(fp);
		exit(0);
	}

#ifdef WIN32
	fstat(_fileno(fp), &stat_buf);
#else
	fstat(fileno(fp), &stat_buf);
#endif

	screw_datalen = stat_buf.st_size - PM9SCREW_LEN;
	screw_datap = (char*)malloc(screw_datalen);
	memset(screw_datap, 0, screw_datalen);
	fread(screw_datap, screw_datalen, 1, fp);
	fclose(fp);

	// 복호화시에는 strlen 필요 없음
	//screw_datalen = strlen(screw_datap); // 암호화시 Filesize가 실제 길이와 달라서 strlen으로 길이 재계산.

	// 복호화데이터 처리
	for(i = 0; i < screw_datalen; i++) {
		screw_datap[i] = (char)pm9screw_mycryptkey[(screw_datalen - i) % cryptkey_len] ^ (~(screw_datap[i]));
	}
	descrew_datap = zdecode(screw_datap, screw_datalen, &descrew_datalen);

	sprintf(descrew_filename, "%s.descrewed", argv[1]);

	fp = fopen(descrew_filename, "w");
	fwrite(descrew_datap, descrew_datalen, 1, fp);
	fclose(fp);

	return 0;
}


