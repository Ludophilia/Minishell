/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jegerman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 18:05:43 by jgermany          #+#    #+#             */
/*   Updated: 2025/10/03 18:07:08 by jegerman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* à organiser plus tard, je vais tout mettre ici puis découper parce que ça fera bcp trop long*/

// pour les differentes formes possibles de -n
static int is_valid_n_option(const char *str)
{
    int i;

    if (!str || str[0] != '-' || str[1] != 'n')
        return (0);
    i = 2;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int bi_echo(t_cmd *cmd, int fd)
{
    int i;
    int newline;
    char **args;

    if (!cmd || !cmd->argv)
        return (-1);

    args = cmd->argv;
    i = 1;
    newline = 1;

    while (args[i] && is_valid_n_option(args[i]))
    {
        newline = 0;
        i++;
    }

    while (args[i])
    {
        ft_putstr_fd(args[i], fd);
        if (args[i + 1])
            ft_putstr_fd(" ", fd);
        i++;
    }

    if (newline)
        ft_putstr_fd("\n", fd);

    return (0);
}

#include <limits.h>

int bi_pwd(int fd)
{
    char cwd[PATH_MAX]; // limit de la taille du path

    if (!getcwd(cwd, PATH_MAX))
    {
        perror("getcwd() error");
        return (1);
    }
    ft_putendl_fd(cwd, fd);
    return (0);
}

// à ajouter dans le header
// liste chainee pour pouvoir parcours facilement l'env et ajouter ou supprimer des lignes ez

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

t_env *env_new(char *key, char *value)
{
    t_env *node;

    node = malloc(sizeof(t_env));
    if (!node)
        return (NULL);
    node->key = key;
    node->value = value;
    node->next = NULL;
    return (node);
}

void env_add(t_env **list, t_env *new)
{
    t_env *last;

    if (!*list)
    {
        *list = new;
        return;
    }
    last = *list;
    while (last->next)
        last = last->next;
    last->next = new;
}

// faire une copie de l'env dans lequel on lance minishell pour pouvoir le modifier avec export et unset
// je pensais à la base qu'on devait créer un env de 0 mais ça serait trop compliqué donc ça va au final

t_env *dup_env(char **envp)
{
    t_env *env_list;
    t_env *new_node;
    char *sep;
    char *key;
    char *value;

    env_list = NULL;
    while (*envp)
    {
        sep = ft_strchr(*envp, '=');
        if (!sep)
        {
            envp++;
            continue;
        }
        key = ft_substr(*envp, 0, sep - *envp);
        value = ft_strdup(sep + 1);
        new_node = env_new(key, value);
        if (!new_node)
            return (NULL);
        env_add(&env_list, new_node);
        envp++;
    }
    return (env_list);
}

// créer l'environnement

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

// qd je ferme le shell, jsp encore ou la mettre

void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

int ft_atol_safe(const char *str, long *res)
{
    unsigned long val = 0;
    long sign = 1;
    int i = 0;

    if (!str || !*str)
        return (0);
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        int digit = str[i] - '0';
        if (val > (LONG_MAX - digit) / 10)
            return (0); // overflow
        val = val * 10 + digit;
        i++;
    }
    *res = (long)(val * sign);
    return (1);
}
void cleanup_and_exit(t_core *core, int code)
{
    utl_cleanup(core->flags, core); // nettoie les commandes
    // free_env(core->env); // nettoie l'environnement, je dois intégrer env dans core
    // rl_clear_history();  // nettoie l'historique readline, à ajouter
    exit(code);
}


// int bi_exit(char *line)
// {
//     printf("exit\n");
//     if (line)
//         free(line);
// 	// exit(EXIT_SUCCESS);
// 	// utl_cleanup(t_cflg flags, t_core *core);
//     return (0);
// }

int bi_exit(t_core *core, t_cmd *cmd)
{
    char **argv;
    long code;

    argv = cmd->argv;
    ft_putendl_fd("exit", STDERR_FILENO);
    if (!argv[1])
        cleanup_and_exit(core, g_exit_status);
    if (!ft_atol_safe(argv[1], &code))
    {
        ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
        ft_putstr_fd(argv[1], STDERR_FILENO);
        ft_putendl_fd(": numeric argument required", STDERR_FILENO);
        cleanup_and_exit(core, 255);
    }
    if (argv[2])
    {
        ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
        g_exit_status = 1;
        return (1);
    }
    cleanup_and_exit(core, code);
    return (0);
}

// fonction pour obtenir la valeur d'une variable d'environnement
char *get_env(t_env *env, const char *key)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return (env->value);
        env = env->next;
    }
    return (NULL);
}

// fonction pour définir ou mettre à jour une variable d'environnement
void set_env(t_env **env, const char *key, const char *value)
{
    t_env *tmp;
    t_env *new;

    tmp = *env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            free(tmp->value);
            tmp->value = ft_strdup(value);
            return;
        }
        tmp = tmp->next;
    }
    // Si on arrive ici, la variable n'existe pas
    malloc(sizeof(t_env));
    if (!new)
        return;
    new->key = ft_strdup(key);
    new->value = ft_strdup(value);
    new->next = *env;
    *env = new;
}

// fonction pour supprimer une variable d'environnement
void unset_env(t_env **env, const char *key)
{
    t_env *tmp;
    t_env *prev;

    prev = NULL;
    tmp = *env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
        {
            if (prev)
                prev->next = tmp->next;
            else
                *env = tmp->next;
            free(tmp->key);
            free(tmp->value);
            free(tmp);
            return;
        }
        prev = tmp;
        tmp = tmp->next;
    }
}

static char *cd_get_path(t_core *core, char **argv)
{
    if (!argv[1]) // cas: cd → HOME
        return get_env(core->env, "HOME");
    if (ft_strcmp(argv[1], "-") == 0) // cas: cd -
        return get_env(core->env, "OLDPWD");
    return argv[1]; // cas général
}

static int cd_update_env(t_core *core, char *oldpwd)
{
    char newpwd[PATH_MAX];

    if (!getcwd(newpwd, sizeof(newpwd)))
    {
        perror("cd: getcwd");
        g_exit_status = 1;
        return (1);
    }
    set_env(&core->env, "OLDPWD", oldpwd);
    set_env(&core->env, "PWD", newpwd);
    return (0);
}

int bi_cd(t_core *core, t_cmd *cmd)
{
    char oldpwd[PATH_MAX];
    char *path;

    if (!getcwd(oldpwd, sizeof(oldpwd)))
        return (perror("cd: getcwd"), g_exit_status = 1, 1);
    path = cd_get_path(core, cmd->argv);
    if (!path)
    {
        ft_putendl_fd("minishell: cd: HOME/OLDPWD not set", STDERR_FILENO);
        return (g_exit_status = 1, 1);
    }
    if (ft_strcmp(cmd->argv[1] ? cmd->argv[1] : "", "-") == 0)
        ft_putendl_fd(path, STDOUT_FILENO);
    if (chdir(path) == -1)
        return (perror("minishell: cd"), g_exit_status = 1, 1);
    return cd_update_env(core, oldpwd);
}

// Vérifie si une clé est valide (commence par lettre/_ puis alphanum/_)
int is_valid_identifier(const char *str)
{
    if (!str || (!ft_isalpha(*str) && *str != '_'))
        return (0);
    for (int i = 1; str[i] && str[i] != '='; i++)
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
    }
    return (1);
}

// Affichage de export sans arguments
static void print_export(t_env *env)
{
    t_env *tmp;

    tmp = env;
    while (tmp)
    {
        ft_putstr_fd("declare -x ", STDOUT_FILENO);
        ft_putstr_fd(tmp->key, STDOUT_FILENO);
        if (tmp->value)
        {
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(tmp->value, STDOUT_FILENO);
            ft_putstr_fd("\"", STDOUT_FILENO);
        }
        ft_putstr_fd("\n", STDOUT_FILENO);
        tmp = tmp->next;
    }
}

// builtin export
int bi_export(t_cmd *cmd, t_env **env)
{
    char *arg;
    char *eq;
    int i;

    i = 1;
    if (!cmd->argv[1]) // juste "export"
    {
        print_export(*env);
        return (0);
    }
    while ((arg = cmd->argv[i]))
    {
        if (!is_valid_identifier(arg))
        {
            ft_putstr_fd("minishell: export: `", STDERR_FILENO);
            ft_putstr_fd(arg, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            g_exit_status = 1;
        }
        else
        {
            eq = ft_strchr(arg, '=');
            if (eq) // forme VAR=value
            {
                *eq = '\0';
                set_env(env, arg, eq + 1);
                *eq = '=';
            }
            else // juste VAR
            {
                if (!get_env(*env, arg))
                    set_env(env, arg, "");
            }
        }
        i++;
    }
    return (0);
}

int bi_unset(t_cmd *cmd, t_env **env)
{
    int i;
    char *arg;

    i = 1;
    while ((arg = cmd->argv[i]))
    {
        if (!is_valid_identifier(arg))
        {
            ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
            ft_putstr_fd(arg, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            g_exit_status = 1;
        }
        else
            unset_env(env, arg);
        i++;
    }
    return (0);
}
