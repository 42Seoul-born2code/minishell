#include "builtin.h"
#include "execute.h"
/*
    Invalid env_name -> exit_code = EXIT_FAILURE
    Invalid option -> exit_code = 2
*/

static void remove_env(t_list *target, t_env_list **env_list)
{
    t_list  *prev_node;
    t_list  *curr_node;
    t_list  *next_node;
    t_env_node  *target_env_node;

    prev_node = NULL;
    curr_node = (*env_list)->head_node;
    next_node = NULL;
    if (curr_node == target)
        (*env_list)->head_node = target->next;
    else
    {
        while (curr_node != NULL)
        {
            if (curr_node == target)
                break ;
            prev_node = curr_node;
            next_node = curr_node->next;
            curr_node = curr_node->next;
        }
        prev_node->next = next_node;
        target_env_node = curr_node->content;
        free(target_env_node->key);
        free(target_env_node->value);
        free(target_env_node);
        free(target);
    }
}

static t_list *get_env(char *key, t_env_list *env_list)
{
    t_list  *list_node;
    t_env_node *env_node;

    list_node = env_list->head_node;
    while (list_node)
    {
        env_node = list_node->content;
        if (ft_strcmp(env_node->key, key) == 0)
            return (list_node);
        list_node = list_node->next;
    }
    return (NULL);
}

int ft_unset(char **argv, t_env_list *env_list)
{
    t_list *target;

    // argv 두번째 값이 널이면 그냥 리턴 (옵션 존재 안할때)
    if (!*(argv + 1))
        return (EXIT_SUCCESS);
    argv++;
    while (*argv)
    {
        // 옵션이 있는 경우 에러
        if (check_option(*argv) == EXIT_FAILURE)
            return (print_error(SYNTAX_ERROR, NULL));
        // argv가 = 인경우 에러처리
        else if (ft_strchr(*argv, '='))
            return (print_error(SYNTAX_ERROR, NULL));
        // 유효하지 않은 환경변수 key는 에러처리
        else if (is_valid_variable_name(*argv) == FALSE)
            return (print_error(SYNTAX_ERROR, NULL));
        else
        {
            target = get_env(*argv, env_list);
            if (target != NULL)
                remove_env(target, &env_list);
        }
        argv++;
    }
    return (EXIT_SUCCESS);
}
