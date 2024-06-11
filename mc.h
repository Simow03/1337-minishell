typedef struct s_item
{
	int				id;
	char			*name;
	int				craftable;
	int				stackSize;
	
	struct s_item	*next;
}	t_item;

