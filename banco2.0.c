#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CLIENTES 50
#define MAX_SAQUES 100

// Estruturas e variáveis globais para armazenar dados dos clientes
char cpfs[MAX_CLIENTES][15];
char contas_correntes[MAX_CLIENTES][12];
float saques_realizados[MAX_CLIENTES][MAX_SAQUES]; // Até 100 saques por cliente
int total_clientes = 0;

// Variáveis globais para armazenar o estado do caixa eletrônico
int cedulas_500 = 100;
int cedulas_150 = 200;
int cedulas_50 = 400;
int cedulas_20 = 800;
int cedulas_10 = 1600;
int cedulas_5 = 3200;
int cedulas_2 = 6400;
int cedulas_1 = 12800;
float saldo_caixa = 173600.00; // Valor total em reais disponível no caixa eletrônico

// Função para converter números em texto
char *geraExtenso(int quantidadeSacar)
{
    static char extenso[200];
    strcpy(extenso, ""); // Limpar o conteúdo da string

    char *unidades[] = {"", "Um", "Dois", "Tres", "Quatro", "Cinco", "Seis", "Sete", "Oito", "Nove"};
    char *dezenas[] = {"", "Dez", "Vinte", "Trinta", "Quarenta", "Cinquenta", "Sessenta", "Setenta", "Oitenta", "Noventa"};
    char *centenas[] = {"", "Cento", "Duzentos", "Trezentos", "Quatrocentos", "Quinhentos", "Seiscentos", "Setecentos", "Oitocentos", "Novecentos"};
    char *dezenas_especiais[] = {"Dez", "Onze", "Doze", "Treze", "Quatorze", "Quinze", "Dezesseis", "Dezessete", "Dezoito", "Dezenove"};
    char *milhares[] = {"", "Mil", "Dois Mil", "Tres Mil", "Quatro Mil", "Cinco Mil", "Seis Mil", "Sete Mil", "Oito Mil", "Nove Mil"};
    char *milhoes[] = {"", "Um Milhao", "Dois Milhoes", "Tres Milhoes", "Quatro Milhoes", "Cinco Milhoes", "Seis Milhoes", "Sete Milhoes", "Oito Milhoes", "Nove Milhoes"};

    if (quantidadeSacar == 0)
    {
        strcpy(extenso, "Zero");
    }
    else
    {
        int n1 = quantidadeSacar % 10;
        int n2 = (quantidadeSacar / 10) % 10;
        int n3 = (quantidadeSacar / 100) % 10;
        int n4 = (quantidadeSacar / 1000) % 10;
        int n5 = (quantidadeSacar / 10000) % 10;
        int n6 = (quantidadeSacar / 100000) % 10;
        int n7 = (quantidadeSacar / 1000000) % 10;

        if (n7 > 0)
        {
            strcat(extenso, milhoes[n7]);
            if (n6 + n5 + n4 + n3 + n2 + n1 > 0)
            {
                strcat(extenso, " e ");
            }
        }

        if (n6 > 0)
        {
            strcat(extenso, centenas[n6]);
            if (n5 == 0 && n4 == 0)
            {
                strcat(extenso, " Mil");
            }
            else if (n5 + n4 > 0)
            {
                strcat(extenso, " e ");
            }
        }

        if (n5 == 1 && n4 > 0)
        {
            strcat(extenso, dezenas_especiais[n4]);
            strcat(extenso, " Mil");
        }
        else
        {
            if (n5 > 0)
            {
                strcat(extenso, dezenas[n5]);
                if (n4 == 0)
                {
                    strcat(extenso, " Mil");
                }
                else
                {
                    strcat(extenso, " e ");
                }
            }
            if (n4 > 0)
            {
                strcat(extenso, milhares[n4]);
            }
        }

        if (n3 > 0)
        {
            if (n7 + n6 + n5 + n4 > 0 && n2 + n1 > 0)
            {
                strcat(extenso, " e ");
            }
            strcat(extenso, centenas[n3]);
        }

        if (n2 == 1 && n1 > 0)
        {
            if (n7 + n6 + n5 + n4 + n3 > 0)
            {
                strcat(extenso, " e ");
            }
            strcat(extenso, dezenas_especiais[n1]);
        }
        else
        {
            if (n2 > 0)
            {
                if (n7 + n6 + n5 + n4 + n3 > 0)
                {
                    strcat(extenso, " e ");
                }
                strcat(extenso, dezenas[n2]);
            }
            if (n1 > 0)
            {
                if (n7 + n6 + n5 + n4 + n3 + n2 > 0)
                {
                    strcat(extenso, " e ");
                }
                strcat(extenso, unidades[n1]);
            }
        }
    }

    return extenso;
}

char geraAlfabeto()
{
    int i;
    char letras[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    i = rand() % 26;
    return (letras[i] - 32); // -32 para converter a letra para maiuscula
}

// Funcao que escolhe um numero aleatorio entre '0' e '9'
char geraNumero()
{
    int i;
    char numeros[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    i = rand() % 10;
    return (numeros[i]);
}

// Funcao para gerar um numero de conta corrente
void geraContaCorrente(char c[])
{
    for (int i = 0; i < 6; i++)
    {
        c[i] = geraNumero();
    }
    c[6] = '.';
    for (int i = 7; i < 10; i++)
    {
        c[i] = geraNumero();
    }
    c[10] = geraAlfabeto();
    c[11] = '\0';
}

// Funcao que calcula o primeiro digito verificador de um CPF
int obtem_primeiro_digito_verificador(char cpf[])
{
    int soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpf[i] - '0') * (10 - i);
    }
    int digito = 11 - (soma % 11);
    return (digito > 9) ? 0 : digito;
}

// Funcao que calcula o segundo digito verificador de um CPF
int obtem_segundo_digito_verificador(char cpf[])
{
    int soma = 0;
    for (int i = 0; i < 9; i++)
    {
        soma += (cpf[i] - '0') * (11 - i);
    }
    soma += obtem_primeiro_digito_verificador(cpf) * 2;
    int digito = 11 - (soma % 11);
    return (digito > 9) ? 0 : digito;
}

// Funcao para gerar um CPF valido
void gera_cpf_valido(char cpf[])
{
    char cpf_sem_digito[10]; // inclui espaco para o '\0'
    for (int i = 0; i < 9; i++)
    {
        cpf_sem_digito[i] = geraNumero();
    }
    cpf_sem_digito[9] = '\0';

    int digito1 = obtem_primeiro_digito_verificador(cpf_sem_digito);
    int digito2 = obtem_segundo_digito_verificador(cpf_sem_digito);

    snprintf(cpf, 12, "%s%d%d", cpf_sem_digito, digito1, digito2); // CPF completo (sem pontuacao)
}

int verifica_cpf_valido(char cpf[])
{

    int valido = 1;
    int digito1 = obtem_primeiro_digito_verificador(cpf);
    int digito2 = obtem_segundo_digito_verificador(cpf);

    int aux_valores = 0;
    for (int i = 0; i < 9; i++)
    {
        aux_valores += ((cpf[i] - '0') * (i + 1));
    }

    int aux_valores1 = 0;

    for (int i = 0; i < 9; i++)
    {
        aux_valores1 += ((cpf[i] - '0') * i);
    }

    int aux_resto1 = 0;
    int aux_resto2 = 0;

    aux_resto2 = aux_valores1 % 11;
    aux_resto1 = aux_valores % 11;

    if (aux_resto1 == 10)
    {
        aux_resto1 = 0;
    }

    if (aux_resto2 == 10)
    {
        aux_resto2 = 0;
    }

    if (aux_resto1 == digito1)
    {
        if (aux_resto2 == digito2)
        {
            return valido;
        }
    }
    else
    {
        printf("\n\n\n Cpf Gerado é invalido! \n");
        return 0;
    }
}

// Funcao que insere pontuacao no CPF
void insere_pontuacao_cpf(char cpf_origem[], char cpf_destino[])
{
    snprintf(cpf_destino, 15, "%.3s.%.3s.%.3s-%s", cpf_origem, cpf_origem + 3, cpf_origem + 6, cpf_origem + 9);
}

// Função para incluir um novo cliente
void incluir_cliente()
{
    if (total_clientes >= MAX_CLIENTES)
    {
        printf("Limite de clientes atingido!\n");
        return;
    }

    char cpf[12];
    char cpf_formatado[15];
    char conta_corrente[12];

    gera_cpf_valido(cpf);

    while (verifica_cpf_valido(cpf) != 1)
    {
        gera_cpf_valido(cpf);
    }

    // Verifica se o CPF gerado e valido

    insere_pontuacao_cpf(cpf, cpf_formatado);
    geraContaCorrente(conta_corrente);

    // Verificar se o CPF ou conta corrente ja existe
    for (int i = 0; i < total_clientes; i++)
    {
        if (strcmp(cpfs[i], cpf_formatado) == 0 || strcmp(contas_correntes[i], conta_corrente) == 0)
        {
            printf("CPF ou conta corrente ja cadastrados!\n");
            return;
        }
    }

    printf("Novo cliente:\nCPF: %s\nConta Corrente: %s\n", cpf_formatado, conta_corrente);
    printf("Deseja confirmar a inclusao? (s/n): ");
    char confirmacao;

    while (confirmacao != 's' && confirmacao != 'S' && confirmacao != 'n' && confirmacao != 'N')
    {
        scanf(" %c", &confirmacao);
    }
    if (confirmacao == 's' || confirmacao == 'S')
    {
        strcpy(cpfs[total_clientes], cpf_formatado);
        strcpy(contas_correntes[total_clientes], conta_corrente);
        memset(saques_realizados[total_clientes], 0, sizeof(saques_realizados[total_clientes]));
        total_clientes++;
        printf("Cliente incluido com sucesso!\n");
    }
    else
    {
        printf("Inclusao cancelada.\n");
    }
}

// Função para mostrar clientes cadastrados
void mostrar_clientes()
{
    if (total_clientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    for (int i = 0; i < total_clientes; i++)
    {
        printf("Cliente %d:\n", i + 1);
        printf("CPF: %s\n", cpfs[i]);
        printf("Conta Corrente: %s\n", contas_correntes[i]);
        printf("\n");
    }
}

// Função para alterar um cliente
void alterar_cliente()
{
    for (int i = 0; i < total_clientes; i++)
    {
        printf("Cliente %d:\n", i + 1);
        printf("CPF: %s\n", cpfs[i]);
        printf("Conta Corrente: %s\n", contas_correntes[i]);
        printf("\n");
    }

    char conta_corrente[12];
    printf("Digite a conta corrente do cliente a ser alterado: ");
    scanf("%s", conta_corrente);

    for (int i = 0; i < total_clientes; i++)
    {
        if (strcmp(contas_correntes[i], conta_corrente) == 0)
        {
            if (saques_realizados[i][0] != 0)
            { // Se houve saque, não pode alterar
                printf("Cliente ja realizou saques. Alteracao nao permitida.\n");
                return;
            }

            char novo_cpf[15], nova_conta_corrente[12];
            gera_cpf_valido(novo_cpf);
            geraContaCorrente(nova_conta_corrente);

            printf("Novo CPF: %s\nNova Conta Corrente: %s\n", novo_cpf, nova_conta_corrente);
            printf("Deseja confirmar a alteracao? (s/n): ");
            char confirmacao;
            scanf(" %c", &confirmacao);

            if (confirmacao == 's' || confirmacao == 'S')
            {
                strcpy(cpfs[i], novo_cpf);
                strcpy(contas_correntes[i], nova_conta_corrente);
                printf("Cliente alterado com sucesso!\n");
            }
            else
            {
                printf("Alteracao cancelada.\n");
            }
            return;
        }
    }
    printf("Conta corrente nao encontrada.\n");
}
// Função para excluir um cliente
void excluir_cliente()
{
    for (int i = 0; i < total_clientes; i++)
    {
        printf("Cliente %d:\n", i + 1);
        printf("CPF: %s\n", cpfs[i]);
        printf("Conta Corrente: %s\n", contas_correntes[i]);
        printf("\n");
    }

    char conta_corrente[12];
    printf("Digite a conta corrente do cliente a ser excluido: ");
    scanf("%s", conta_corrente);

    for (int i = 0; i < total_clientes; i++)
    {
        if (strcmp(contas_correntes[i], conta_corrente) == 0)
        {
            if (saques_realizados[i][0] != 0)
            { // Se houve saque, não pode excluir
                printf("Cliente ja realizou saques. Exclusao nao permitida.\n");
                return;
            }

            for (int j = i; j < total_clientes - 1; j++)
            {
                strcpy(cpfs[j], cpfs[j + 1]);
                strcpy(contas_correntes[j], contas_correntes[j + 1]);
                memcpy(saques_realizados[j], saques_realizados[j + 1], sizeof(saques_realizados[j]));
            }
            total_clientes--;
            printf("Cliente excluido com sucesso!\n");
            return;
        }
    }
    printf("Conta corrente nao encontrada.\n");
}

// Função para realizar saque
void realizar_saque()
{
    if (total_clientes == 0)
    {
        printf("Ainda nao ha clientes registrados \n");
    }
    else
    {

        for (int i = 0; i < total_clientes; i++)
        {
            printf("Cliente %d:\n", i + 1);
            printf("CPF: %s\n", cpfs[i]);
            printf("Conta Corrente: %s\n\n", contas_correntes[i]);
        }

        char conta_corrente[12];
        printf("Digite a conta corrente: ");
        scanf("%s", conta_corrente);

        for (int i = 0; i < total_clientes; i++)
        {
            if (strcmp(contas_correntes[i], conta_corrente) == 0)
            {
                float valor;
                printf("Digite o valor do saque: ");
                scanf("%f", &valor);

                if (valor > saldo_caixa)
                {
                    printf("Saldo insuficiente no caixa eletronico.\n");
                    return;
                }

                if (valor <= 0)
                {
                    printf("Saque invalido ou negativo \n");
                }

                int ced_500 = 0, ced_150 = 0, ced_50 = 0, ced_20 = 0, ced_10 = 0, ced_5 = 0, ced_2 = 0, ced_1 = 0;
                float valor_restante = valor, valor_total_sacado = valor_total_sacado + valor;

                while (valor_restante >= 500 && cedulas_500 > 0)
                {
                    valor_restante -= 500;
                    ced_500++;
                    cedulas_500--;
                }
                while (valor_restante >= 150 && cedulas_150 > 0)
                {
                    valor_restante -= 150;
                    ced_150++;
                    cedulas_150--;
                }
                while (valor_restante >= 50 && cedulas_50 > 0)
                {
                    valor_restante -= 50;
                    ced_50++;
                    cedulas_50--;
                }
                while (valor_restante >= 20 && cedulas_20 > 0)
                {
                    valor_restante -= 20;
                    ced_20++;
                    cedulas_20--;
                }
                while (valor_restante >= 10 && cedulas_10 > 0)
                {
                    valor_restante -= 10;
                    ced_10++;
                    cedulas_10--;
                }
                while (valor_restante >= 5 && cedulas_5 > 0)
                {
                    valor_restante -= 5;
                    ced_5++;
                    cedulas_5--;
                }
                while (valor_restante >= 2 && cedulas_2 > 0)
                {
                    valor_restante -= 2;
                    ced_2++;
                    cedulas_2--;
                }
                while (valor_restante >= 1 && cedulas_1 > 0)
                {
                    valor_restante -= 1;
                    ced_1++;
                    cedulas_1--;
                }

                if (valor_restante > 0)
                {
                    printf("Nao ha cedulas suficientes para realizar o saque desse valor.\n");
                    return;
                }

                saldo_caixa -= valor;

                // Registrar o saque na conta do cliente
                for (int j = 0; j < MAX_SAQUES; j++)
                {
                    if (saques_realizados[i][j] == 0)
                    {
                        saques_realizados[i][j] = valor;
                        break;
                    }
                }

                printf("Saque realizado com sucesso! Cedulas entregues:\n");
                printf("R$500,00: %d\nR$150,00: %d\nR$50,00: %d\nR$20,00: %d\nR$10,00: %d\nR$5,00: %d\nR$2,00: %d\nR$1,00: %d\n", ced_500, ced_150, ced_50, ced_20, ced_10, ced_5, ced_2, ced_1);
                return;
            }
        }
        printf("Conta corrente nao encontrada.\n");
    }
}

// Função para relatar valores sacados por cada cliente
void relatorio_valores_sacados()
{
    if (total_clientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    float valor_total_sacado = 0;

    for (int i = 0; i < total_clientes; i++)
    {
        float total_sacado_cliente = 0;
        printf("Cliente %d:\n", i + 1);
        printf("CPF: %s\n", cpfs[i]);
        printf("Conta Corrente: %s\n", contas_correntes[i]);
        printf("Valores sacados:\n");
        for (int j = 0; j < MAX_SAQUES; j++)
        {
            if (saques_realizados[i][j] != 0)
            {
                printf("\nR$%.2f \n", saques_realizados[i][j]);
                total_sacado_cliente += saques_realizados[i][j];
            }
            else
            {
                break;
            }
        }
        printf("\nTotal sacado por este cliente: R$%.2f\n", total_sacado_cliente);

        printf("\n");

        valor_total_sacado += total_sacado_cliente;
    }

    printf("Valor total sacado no caixa: R$%.2f\n", valor_total_sacado);
    printf("(%s)\n", geraExtenso(valor_total_sacado));
}

// Função para relatar o saldo existente no caixa eletrônico
void relatorio_saldo_existente()
{
    printf("Saldo total disponivel no caixa eletronico: R$%.2f\n", saldo_caixa);
    printf("(%s)\n", geraExtenso(saldo_caixa));
}

// Função para exibir o relatório de cédulas existentes
void relatorio_cedulas_existentes()
{
    printf("Cedulas disponiveis:\n");
    printf("R$500,00: %d (%s)\n", cedulas_500, geraExtenso(cedulas_500));
    printf("R$150,00: %d (%s)\n", cedulas_150, geraExtenso(cedulas_150));
    printf("R$50,00: %d (%s)\n", cedulas_50, geraExtenso(cedulas_50));
    printf("R$20,00: %d (%s)\n", cedulas_20, geraExtenso(cedulas_20));
    printf("R$10,00: %d (%s)\n", cedulas_10, geraExtenso(cedulas_10));
    printf("R$5,00: %d (%s)\n", cedulas_5, geraExtenso(cedulas_5));
    printf("R$2,00: %d (%s)\n", cedulas_2, geraExtenso(cedulas_2));
    printf("R$1,00: %d (%s)\n", cedulas_1, geraExtenso(cedulas_1));
}

// Menu principal
int main()
{
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int opcao_principal = 0, opcao_cliente = 0, opcao_relatorio = 0, menu = 0;

    while (1)
    {
        if (menu == 0)
        {
            printf("MENU PRINCIPAL\n");
            printf("1-Cliente\n2-Saque\n3-Relatorios\n4-Finalizar\n");
            printf("Escolha uma opcao: ");

            if (scanf("%d", &opcao_principal) != 1)
            {
                // se o usuario inserir uma letra
                printf("Opcao invalida, apenas digite numeros.\n");
                while (getchar() != '\n')
                    ;     // limpa o buffer
                continue; // Recomeça o loop
            }
        }

        switch (opcao_principal)
        {
        // menu cliente
        case 1:
            system("cls");
            printf("MENU CLIENTE\n");
            printf("1-Incluir\n2-Mostrar\n3-Alterar\n4-Excluir\n5-Voltar\n");
            printf("Escolha uma opcao: ");
            if (scanf("%d", &opcao_cliente) != 1)
            {
                printf("Opcao invalida!\n");
                while (getchar() != '\n')
                    ; // vai limpar o buffer de entrada
                continue;
            }

            switch (opcao_cliente)
            {
            case 1:
                // opcao incluir cliente
                system("cls");
                incluir_cliente();
                system("pause");
                system("cls");
                break;
            case 2:
                // opcao mostrar clientes criados
                system("cls");
                mostrar_clientes();
                system("pause");
                system("cls");
                break;
            case 3:
                // opcao alterar cliente
                system("cls");
                alterar_cliente();
                system("pause");
                system("cls");
                break;
            case 4:
                // olpcoa excluir cliente
                system("cls");
                excluir_cliente();
                system("pause");
                system("cls");
                break;
            case 5:
                // opcao voltar para o menu principal
                system("cls");
                menu = 0;
                break;
            default:
                printf("Opcao invalida!\n");
                break;
            }
            break;

        case 2: // Saque
            system("cls");
            realizar_saque();
            system("pause");
            system("cls");
            break;

        case 3: // Relatorios
            system("cls");
            printf("MENU RELATORIOS\n");
            printf("1-Valores sacados\n2-Valor do saldo existente\n3-Quantidade de cedulas existentes\n4-Voltar ao menu principal\n");
            printf("Escolha uma opcao: ");
            if (scanf("%d", &opcao_relatorio) != 1)
            {
                printf("Opcao invalida!\n");
                while (getchar() != '\n')
                    ; // Limpa o buffer de entrada
                continue;
            }

            switch (opcao_relatorio)
            {
            case 1:
                // opcao relatorio de todos os valores sacados por cada cliente
                system("cls");
                relatorio_valores_sacados();
                system("pause");
                system("cls");
                break;
            case 2:
                // opcao relatorio do valor existente no banco
                system("cls");
                relatorio_saldo_existente();
                system("pause");
                system("cls");
                break;
            case 3:
                // opcao relatorio de cedulas existentes no bacno
                system("cls");
                relatorio_cedulas_existentes();
                system("pause");
                system("cls");
                break;
            case 4:
                // opcao voltar para o menu principal
                system("cls");
                menu = 0;
                break;
            default:
                printf("Opcao invalida!\n");
                break;
            }
            break;

        case 4: // Finalizar programa
            system("cls");
            printf("Encerrando o sistema...\n");
            exit(0);

        default:
            printf("Opcao invalida!\n");
            break;
        }
    }

    return 0;
}