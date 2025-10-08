#include "minishell.h"

// vérifie si l'identifiant est valide
int is_valid_identifier(const char *str)
{
    int i;

    if (!str || (!ft_isalpha(*str) && *str != '_'))
        return (0);
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

// Affichage de export sans arguments
static void	print_export(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(tmp->key, STDOUT_FILENO);
		if (tmp->value)
		{
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(tmp->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
		tmp = tmp->next;
	}
}


// gère chaque argument de export
static void	handle_export_arg(t_env **env, char *arg)
{
	char	*eq;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		g_exit_status = 1;
		return;
	}
	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		set_env(env, arg, eq + 1);
		*eq = '=';
	}
	else if (!get_env(*env, arg))
		set_env(env, arg, "");
}

int	bi_export(t_cmd *cmd, t_env **env)
{
	int	i;

	if (!cmd || !env)
		return (1);
	if (!cmd->argv[1])
    {
        print_export(*env);
        return (0);
    }
	i = 1;
	while (cmd->argv[i])
	{
		handle_export_arg(env, cmd->argv[i]);
		i++;
	}
	return (0);
}

