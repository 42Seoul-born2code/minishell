#include "builtin.h"
/*
    Invalid env_name -> exit_code = EXIT_FAILURE
    Invalid option -> exit_code = 2
*/

static void remove_env(t_list *target, t_env_list *env_list)
{
    t_list  *prev_node;
    t_list  *curr_node;
    t_list  *next_node;
    t_env_node  *target_env_node;

    prev_node = NULL;
    curr_node = env_list->head_node;
    next_node = NULL;
    // 1. 환경변수가 첫번째에 잡혔을 경우
    // head_node 를 target->next 로 이어주기
    if (curr_node == target)
        env_list->head_node = target->next;
    // 2. 중간에 있을 때 + 마지막일 때
    // target->prev 랑 target->next 랑 이어주기
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

static t_bool is_valid_variable_name(char *str)
{
    int idx;

    if (ft_isdigit(str[0]) == TRUE)
        return (FALSE);
    idx = 0;
    while (str[idx] != '\0')
    {
        if (ft_isalnum(str[idx]) == TRUE || str[idx] == '_')
            idx += 1;
        else
            return (FALSE);
    }
    return (TRUE);
}

int ft_unset(char **argv, t_env_list *env_list)
{
    int exit_code;
    t_list *target;

    exit_code = EXIT_SUCCESS;
    // argv 두번째 값이 널이면 그냥 리턴 (옵션 존재 안할때)
    if (!*(argv + 1))
        return (exit_code);
    argv++;
    while (*argv)
    {
        // 옵션이 있는 경우 에러
        if (check_option(*argv) == EXIT_FAILURE)
        {
            exit_code = EXIT_FAILURE;
            printf("%s\n", SYNTAX_ERROR);
        }
        // argv가 = 인경우 에러처리
        else if (ft_strchr(*argv, '='))
        {
            exit_code = EXIT_FAILURE;
            printf("%s\n", SYNTAX_ERROR);
        }
        // 유효하지 않은 환경변수 key는 에러처리
        else if (is_valid_variable_name(*argv) == FALSE)
        {
            exit_code = EXIT_FAILURE;
            printf("%s\n", SYNTAX_ERROR);
        }
        else
        {
            target = get_env(*argv, env_list);
            if (target != NULL)
                remove_env(target, env_list);
        }
        argv++;
    }
    return (exit_code);
}
