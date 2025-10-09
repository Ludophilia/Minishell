/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ntahri <ntahri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 20:29:58 by ntahri            #+#    #+#             */
/*   Updated: 2025/10/09 04:06:47 by ntahri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// crée un nouveau noeud d'environnement
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
// ajoute un noeud à la fin de la liste chaînée
void env_add(t_env **list, t_env *new)
{
    t_env *last;

    if (!list || !new)
        return;
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
// libère toute la liste chaînée
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
// gestion des erreurs lors de la duplication de l'env
static t_env *dup_env_fail(char *key, char *val, t_env *list)
{
    free(key);
    free(val);
    free_env(list);
    return (NULL);
}
// duplique le tableau envp en une liste chaînée
t_env *dup_env(char **envp)
{
    t_env *env_list;
    t_env *new;
    char *sep;
    char *key;
    char *val;

    env_list = NULL;
    while (*envp)
    {
        sep = ft_strchr(*envp, '=');
        if (sep)
        {
            key = ft_substr(*envp, 0, sep - *envp);
            val = ft_strdup(sep + 1);
            if (!key || !val)
                return (dup_env_fail(key, val, env_list));
            new = env_new(key, val);
            if (!new)
                return (dup_env_fail(key, val, env_list));
            env_add(&env_list, new);
        }
        envp++;
    }
    return (env_list);
}
