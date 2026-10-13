#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 *
 */
int	picoshell(char **cmds[])
{
	int fd[2]; // fd[0]=読み口, fd[1]=書き口
	int in;    // 前のパイプの読み口。0なら前のパイプはない (つまり、最初のコマンド)
	int ret;   // 返り値。エラーが起きたら 1
	in = 0;
	ret = 0;
	// cmds[0], cmds[1], ... を NULLまで1つずつ処理する
	for (int i = 0; cmds[i]; i++)
	{
		// 次のコマンドがあるときだけパイプを作る (最後のコマンドの後には不要なのでpipeは作らない)
		if (cmds[i + 1] && pipe(fd) < 0)
			ret = 1;
		if (ret) // pipeに失敗したらループを抜けて後片付けへ
			break ;
		// === 子プロセス: このコマンドになる ===
		if (fork() == 0)
		{
			// 前のパイプがあるなら、そこから読む
			if (in)
				(dup2(in, 0), close(in)); // コピーし終わった元の fd は閉じる
			// 次のコマンドがあるなら、新しいパイプに書く (stdout を差し替える)
			if (cmds[i + 1])
				(dup2(fd[1], 1), close(fd[0]), close(fd[1]));
			// 子は fd[0] (読み口) を使わないので閉じる
			// fd[1] は 1番に コピーしたので元は閉じる
			execvp(cmds[i][0], cmds[i]); // 成功するとここから先には戻ってこない
			exit(1);                     // 戻ってきた = execvp失敗
		}
		// === 親プロセス: 後片付けと次の準備 ===
		// 前のパイプの読み口は今の子に渡したので、親はもう使わない
		if (in)
			close(in);
		in = 0;
		if (cmds[i + 1])
			(close(fd[1]), in = fd[0]);
		// 書き口は子に渡したので閉じる (閉じないと次のコマンドが EOF を受け取れず固まる)
		// 読み口は「次のコマンドの stdin」として取っておく
	}
	// break で抜けた場合とかに、読み口が残っていたら閉じる
	if (in)
		close(in);
	// 子プロセスを全部持つ (wait は子がいなくなると -1 を返す)
	while (wait(NULL) > 0)
		;
	return (ret);
}
