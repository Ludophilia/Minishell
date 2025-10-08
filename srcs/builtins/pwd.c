#include "minishell.h"

int bi_pwd(int fd)
{
    char cwd[PATH_MAX];

    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("pwd");
        return (1);
    }
    ft_putendl_fd(cwd, fd);
    return (0);
}

int bi_env(t_env *env, int fd)
{
    while (env)
    {
        if (env->value)
        {
            ft_putstr_fd(env->key, fd);
            ft_putstr_fd("=", fd);
            ft_putendl_fd(env->value, fd);
        }
        env = env->next;
    }
    return (0);
}
