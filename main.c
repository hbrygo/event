// #include <event.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <readline/readline.h>
#include <readline/history.h>

int	ft_strlen(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		i++;
	}
	return (i);
}

void	*ft_memset(void *b, int c, int len)
{
	int				i;
	unsigned char	*str;

	i = 0;
	str = b;
	while (i < len)
	{
		str[i] = c;
		i++;
	}
	return (str);
}

int	ft_check(char *lettre, char *word)
{
	if (ft_strlen(word) == ft_strlen(lettre))
		return (1);
	return (0);
}

int	ft_is_in(char *test, char lettre)
{
	int	i;

	i = 0;
	while (test[i])
	{
		if (test[i] == lettre)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_tolower(char *str)
{
	int 	i;
	char	*ret;

	ret = malloc(ft_strlen(str) + 1);
	i = 0;
	while (str[i])
	{
		if (str[i] <= 'Z' && str[i] >= 'A')
			ret[i] = str[i] + 32;
		else
			ret[i] = str[i];
		i++;
	}
	ret[i] = 0;
	return(ret);
}

void	ft_aide(int *bonne, int *presque, int len, char *word, char *lettre)
{
	int 	i;
	int 	j;
	int		k;
	int		*place;
	int		*find;

	i = 0;
	k = 0;
	place = malloc(sizeof(int) * len);
	ft_memset(place, 0, len);
	find = malloc(sizeof(int) * len);
	ft_memset(find, 0, len);
	while (i < len)
	{
		if (word[i] == lettre[i])
		{
			place[i] = 1;
			find[i] = 1;
			(*bonne)++;
		}
		i++;
	}
	i = 0;
	while (i < len)
	{
		j = 0;
		while (j < len)
		{
			if (word[i] == lettre[j] && place[i] != 1 && find[j] != 1)
			{
				place[i] = 1;
				(*presque)++;
			}
			j++;
		}
		i++;
	}
}

int	ft_verif(char *word, char *lettre, int len)
{
	int	nb;
	int	i;
	int	bonne;
	int	presque;

	nb = 0;
	i = 0;
	bonne = 0;
	presque = 0;
	if (ft_check(lettre, word) == 0)
	{
		printf("Le mot fait %d lettre\n", len);
		return (0);
	}
	ft_aide(&bonne, &presque, len, word, lettre);
	if (bonne == len)
		return (len);
	printf("Il y a %d lettre bien placee\n", bonne);
	printf("Il y a %d lettre mal placee\n", presque);
	return (nb);
}

char	*ft_strjoin(char *s1, char s2[1])
{
	int		i;
	int		j;
	char	*str;

	i = -1;
	j = -1;
	str = malloc(sizeof(char) * (ft_strlen(s1) + 2));
	if (!str)
		return (NULL);
	while (++i < (int)ft_strlen(s1))
	{
		printf("%d\n", i);
		str[i] = s1[i];
	}
	str[i] = s2[0];
	str[i + 1] = '\0';
	free(s1);
	return (str);
}

int	ft_try(char *word, char **str, int len)
{
	char	*lettre;
	int		lettre_trouve;

	(void)str;
	printf("\033[1;33mEntrer une combinaison de %d lettres: \033[1;32m", len);
	lettre = readline("");
	lettre = ft_tolower(lettre);
	lettre_trouve = ft_verif(word, lettre, len);
	if (lettre_trouve == len)
		return (lettre_trouve);
	free(lettre);
	return (0);
}

void	ft_indice(char **indice)
{
	// choose a random number and write the string in indice
	static int	i = 1;

	printf("\033[1;31mVoici un indice car tu as du mal: %s\n\033[27;32m", indice[i]);
	i++;
}

int	ft_game(char *word, int len, char **indice)
{
	int	vie;
	char	*str;
	int	ret;
	int	score;

	score = 0;;
	str = malloc(len + 1);
	ft_memset(str, '_', len);
	str[len] = 0;
	vie = 20;
	while (vie != 0 && len != score)
	{
		ret = ft_try(word, &str, len);
		if (vie && vie % 5 == 0 && vie != 20)
			ft_indice(indice);
		if (ret != 0)
			score = ret;
		else
			vie--;
	}
	if (vie == 0)
		return (1);
	return (0);
}

static int	count_strings(char *str, char charset)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (str[i] != '\0')
	{
		while (str[i] != '\0' && str[i] == charset)
			i++;
		if (str[i] != '\0')
			count++;
		while (str[i] != '\0' && str[i] != charset)
			i++;
	}
	return (count);
}

static char	*ft_word(char *str, char charset)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	while (str[i] && str[i] != charset)
		i++;
	len_word = i;
	i = 0;
	word = (char *)malloc(sizeof(char) * (len_word + 1));
	if (!word)
		return (NULL);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	*ft_put_word(char *str, char c, int i, char **string)
{
	string[i] = ft_word(str, c);
	if (!string[i])
	{
		while (i >= 0)
		{
			free(string[i]);
			i--;
		}
		return (NULL);
	}
	return (string[i]);
}

static int	ft_set_word(char **strings, char *str, int i, char c)
{
	strings[i] = ft_put_word(str, c, i, strings);
	if (!strings[i])
	{
		free(strings);
		return (0);
	}
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**strings;
	char	*str;
	int		i;

	i = 0;
	str = (char *)s;
	strings = (char **)malloc(sizeof(char *) * (count_strings(str, c) + 1));
	if (!strings)
		return (NULL);
	while (*str != '\0')
	{
		while (*str && *str == c)
			str++;
		if (*str)
		{
			if (ft_set_word(strings, str, i, c) == 0)
				return (NULL);
			i++;
		}
		while (*str && *str != c)
			str++;
	}
	strings[i] = 0;
	return (strings);
}

int main(int argc, char **argv)
{
	char	*word;
	char	**final;
	char	c;
	int		len;
	int		ret;
	int		i;
	FILE *fd;

	word = malloc(255);
	ft_memset(word, 0, 255);
	if (argc != 2)
		return (0);
	fd = fopen(argv[1], "r");
	i = -1;
	while ((ret = fscanf(fd, "%c", &c)) != -1)
		word[++i] = c;
	word[i + 1] = '\0';
	final = ft_split(word, '\n');
	ft_strlen(final[0]);
	final[0] = ft_tolower(final[0]);
	len = ft_strlen(final[0]);
	if (ft_game(final[0], len, final) == 0)
		return (printf("Bravo: vous avez touvez le mot %s\n", final[0]));
	return (printf("Dommage: le mot etait %s\n", final[0]));
}
