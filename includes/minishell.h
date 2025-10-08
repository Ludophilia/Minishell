/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 13:50:47 by jegerman          #+#    #+#             */
/*   Updated: 2025/10/08 23:15:04 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft.h"

#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

#define UI_MAGENTA "\033[1;35m"
#define UI_RESET "\033[0m"
#define UI_PROMPT "\033[1;35mMinishell> \033[0m"

#define ENV_DFLT_PATH "/bin:/usr/bin"

#define ERR_SYNTAX "syntax error near unexpected token `%s'\n"
#define ERR_GNR "%s\n"
#define ERR_PTH "%s: %s\n"
#define ERR_CMD "%s: command not found\n"
#define ERR_ECMD "'%c': command not found\n"

#define FL_PRMS 00664
#define FL_IRED O_RDONLY
#define FL_ORED (O_CREAT | O_TRUNC | O_WRONLY)
#define FL_ORED_AP (O_CREAT | O_APPEND | O_WRONLY)

#define PATH_MAX 4096

extern uint32_t g_exit_status;

typedef struct sigaction t_sigaction;
typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;
typedef enum e_max
{
    TOK_MAX = 2048,
    CMD_MAX = 128,
    RED_MAX = 128,
    ID_LMAX = 256,
    EXW_MAX = 16,
} t_max;

typedef enum e_tokt
{
    TOK_EOL,
    TOK_WORD,
    TOK_PIPE,
    TOK_IRED,
    TOK_IRED_HD,
    TOK_ORED,
    TOK_ORED_AP
} t_tokt;

typedef enum e_cflg
{
    FLG_CMDS = (1 << 0),
    FLG_REDS = (1 << 1),
    FLG_CORE = (1 << 2),
    FLG_ALL = (FLG_CMDS | FLG_REDS | FLG_CORE)
} t_cflg;

typedef struct s_tok
{
    enum e_tokt type;
    char *start;
    int len;
} t_tok;

typedef struct s_red
{
    t_tokt type;
    char *word;
    int fds[PFD_MAX];
} t_red;

typedef struct s_cmd
{
    char **argv;
    pid_t pid;
    t_red reds[RED_MAX];
    int ifd;
    int ofd;
    bool xready;
} t_cmd;
typedef struct s_core
{
    t_cmd cmds[CMD_MAX];
    int cmd_pos;
    int cmd_pmax;
    int cmd_xrdy;
    uint32_t flags;
    uint8_t exitv;
    t_env *env;
} t_core;

int lex_is_quote(int c);
int lex_is_op(int c);
int lex_is_sep(int c);

int lex_tokenize_line(char *line, t_tok *toks);

int psr_cleanup_red(t_red *red, int *fds);
// int psr_cleanup_cmds(t_core *core);
int psr_cleanup_cmds(t_cflg flags, t_core *core);

int psr_is_ired(t_tok *tok);
int psr_is_ored(t_tok *tok);
int psr_is_outq(int c, int *q);
int psr_is_envv(char *c, int ct, int q);
int psr_is_envv_chr(int c, int pos);

char *psr_create_word(t_tok *tok, t_tokt context);
int psr_add_reds(t_tok *tok, t_cmd *cmd);
int psr_add_cmd(t_tok *tok, t_cmd *cmd);
int psr_error_check(t_tok *toks);
int psr_parse_line(char *line, t_core *core);

int utl_cleanup(t_cflg flags, t_core *core);
char *utl_shitoa(unsigned int nbr, char *store);

int fmgr_access(char *path, int type);
int fmgr_open(char *path, int openflags, mode_t openmode);
int fmgr_pipe(int fds[2]);
int fmgr_close(int *xfd);
int fmgr_dup2(int old_fd, int new_fd);

int fmgr_set_hdocs(int *ifds, t_red *red);
int fmgr_set_pipe(int pos, int pmax, t_cmd *cmd);
int fmgr_set_red(int *xfd, int openflags, t_red *red);
int fmgr_set_reds(t_core *core);

int utl_free_strs(int from_id, char **strs);
int utl_cleanup(t_cflg flags, t_core *core);
char *utl_shitoa(unsigned int nbr, char *store);
int utl_free(void *ptr);

int exc_is_builtin(char *arg);
int exc_check_path(char **argv, char **envp);
int exc_exec_cmds(t_core *core);

int sig_init_handlers(void);

int bi_echo(t_cmd *cmd, int fd);
int bi_cd(t_core *core, t_cmd *cmd);
int bi_pwd(int fd);
int bi_env(t_env *env, int fd);
int bi_export(t_cmd *cmd, t_env **env);
int bi_unset(t_cmd *cmd, t_env **env);
int bi_exit(t_core *core, t_cmd *cmd);

t_env *env_new(char *key, char *value);
void env_add(t_env **list, t_env *new);
t_env *dup_env(char **envp);
char *get_env(t_env *env, const char *key);
void set_env(t_env **env, const char *key, const char *value);
void unset_env(t_env **env, const char *key);
void free_env(t_env *env);
int is_valid_identifier(const char *str);

int ft_atol_safe(const char *str, long *res);
void cleanup_and_exit(t_core *core, int code);

int ui_loop_prompt(t_core *core);

#endif
