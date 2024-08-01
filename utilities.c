int ft_atoi(char *str) {
    int result;
    int sign;

    result = 0;
    if(!str)
        return 0;
    if(*str == '-') {
        sign = -1;
        str++;
    }
    else {
        sign = 1;
    }
    while(*str) {
        result *= 10;
        result += *str - '0';
        str++;
    }
    return (result*sign);
}