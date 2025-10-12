#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

typedef struct s_nbr
{
	int				sign;
	unsigned long	val;
}	t_nbr;


// static int	ft_atol_safe(const char *str, long *res)
// {
// 	int				sign;
// 	unsigned long	val;
// 	int				digit;

// 	val = 0;
// 	sign = 1;
// 	if (!str || !*str)
// 		return (0);
// 	if ((*str == '+' || *str == '-') && *str++ == '-')
// 		sign = -1;
// 	while (*str)
// 	{
// 		if (!isdigit(*str))
// 			return (0);
// 		digit = *str - '0';
// 		val = val * 10 + digit;
// 		str++;
// 	}
// 	*res = (long)(val * sign);
// 	return (1);
// }

int	ft_atol_safe(const char *str, long *res)
{
	unsigned long	val;
	long			sign;
	int				digit;

	val = 0;
	sign = 1;
	if (!str || !*str)
		return (0);
	if ((*str == '+' || *str == '-') && *str++ == '-')
		sign = -1;
	while (*str)
	{
		if (!isdigit(*str))
			return (0);
		digit = *str - '0';
		// if (val > (LONG_MAX - digit) / 10)
		// 	return (0);
		val = val * 10 + digit;
		str++;
	}
	*res = (long)(val * sign);
	return (1);
}

int	main(void)
{
	long	res;
	char	*nbr;

	res = 0;
	nbr = "-922a3372036854775808";
	// nbr = "9223372036854775807";
	printf("ret -> %i\n", ft_atol_safe(nbr, &res));
	printf("res -> %li\n", res);
	return (0);
}
