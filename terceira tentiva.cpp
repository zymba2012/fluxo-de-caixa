#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct Produto {
    char nome[50];
    float preco;
    int codigo;
};

struct Funcionario {
    char nome[50];
    char dataAdmissao[20];
    int matricula;
    float comissao;
};

struct Cliente {
    char nome[50];
    int idade;
    float vendas;
};

struct Venda {
    struct Cliente cliente;
    struct Funcionario funcionario;
    float valorTotal;
};

int gerarCodigo() {
    return rand() % 9000 + 1000;
}

int gerarMatricula() {
    return rand() % 900000 + 100000;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void cadastrarProduto(struct Produto *produto) {
    printf("Digite o nome do produto: \n");
    fgets(produto->nome, sizeof(produto->nome), stdin);
    produto->nome[strcspn(produto->nome, "\n")] = '\0';  // Remove a quebra de linha final
    printf("Digite o pre�o do produto: ");
    scanf("%f", &produto->preco);
    produto->codigo = gerarCodigo();
}

void cadastrarFuncionario(struct Funcionario *funcionario) {
    printf("Digite o nome do funcion�rio: ");
    scanf("%s", funcionario->nome);
    printf("Digite a data de admiss�o do funcion�rio: ");
    scanf("%s", funcionario->dataAdmissao);
    funcionario->matricula = gerarMatricula();
    funcionario->comissao = 0.0;
}

void cadastrarCliente(struct Cliente *cliente) {
    printf("Digite o nome do cliente: ");
    scanf("%s", cliente->nome);
    printf("Digite a idade do cliente: ");
    scanf("%d", &cliente->idade);
    cliente->vendas = 0.0;
}
void cadastrarProduto(struct Produto *produto) {
    printf("Digite o nome do produto: \n");
    limparBufferEntrada();
    fgets(produto->nome, sizeof(produto->nome), stdin);
    produto->nome[strcspn(produto->nome, "\n")] = '\0';  // Remove a quebra de linha final
    printf("Digite o pre�o do produto: ");
    scanf("%f", &produto->preco);
    produto->codigo = gerarCodigo();
}
void carregarProdutos(struct Produto *produtos, int *numProdutos) {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo de produtos para leitura");
        return;
    }

    while (fscanf(arquivo, "%s %f %d", produtos[*numProdutos].nome, &produtos[*numProdutos].preco, &produtos[*numProdutos].codigo) == 3) {
        (*numProdutos)++;
    }

    fclose(arquivo);
}


void salvarFuncionarios(struct Funcionario *funcionarios, int numFuncionarios) {
    FILE *arquivo = fopen("funcionarios.txt", "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de funcion�rios para escrita.\n");
        return;
    }

    for (int i = 0; i < numFuncionarios; i++) {
        fprintf(arquivo, "%s %s %d %.2f\n", funcionarios[i].nome, funcionarios[i].dataAdmissao, funcionarios[i].matricula, funcionarios[i].comissao);
    }

    fclose(arquivo);
}

void carregarFuncionarios(struct Funcionario *funcionarios, int *numFuncionarios) {
    FILE *arquivo = fopen("funcionarios.txt", "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de funcion�rios para leitura.\n");
        return;
    }

    while (fscanf(arquivo, "%s %s %d %f", funcionarios[*numFuncionarios].nome, funcionarios[*numFuncionarios].dataAdmissao, &funcionarios[*numFuncionarios].matricula, &funcionarios[*numFuncionarios].comissao) == 4) {
        (*numFuncionarios)++;
    }

    fclose(arquivo);
}

void salvarClientes(struct Cliente *clientes, int numClientes) {
    FILE *arquivo = fopen("clientes.txt", "w");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de clientes para escrita.\n");
        return;
    }

    for (int i = 0; i < numClientes; i++) {
        fprintf(arquivo, "%s %d %.2f\n", clientes[i].nome, clientes[i].idade, clientes[i].vendas);
    }

    fclose(arquivo);
}

void carregarClientes(struct Cliente *clientes, int *numClientes) {
    FILE *arquivo = fopen("clientes.txt", "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo de clientes para leitura.\n");
        return;
    }

    while (fscanf(arquivo, "%s %d %f", clientes[*numClientes].nome, &clientes[*numClientes].idade, &clientes[*numClientes].vendas) == 3) {
        (*numClientes)++;
    }

    fclose(arquivo);
}

void registrarVenda(struct Venda *venda, struct Produto *produtos, int numProdutos,
                    struct Cliente *clientes, int numClientes,
                    struct Funcionario *funcionarios, int numFuncionarios) {
    int opcaoCliente;
    printf("Escolha a op��o (1 - Cliente Cadastrado, 2 - Cliente Avulso): ");
    scanf("%d", &opcaoCliente);

    if (opcaoCliente == 1) {
        int codigoCliente;
        printf("Digite o c�digo do cliente cadastrado: ");
        scanf("%d", &codigoCliente);

        // Encontrar cliente pelo c�digo
        int clienteEncontrado = 0;
        for (int i = 0; i < numClientes; i++) {
            if (clientes[i].vendas == codigoCliente) {
                venda->cliente = clientes[i];
                clienteEncontrado = 1;
                break;
            }
        }

        if (!clienteEncontrado) {
            printf("Cliente n�o encontrado. Registrando como cliente avulso.\n");
            opcaoCliente = 2; // Registrar como cliente avulso
        }
    }

    if (opcaoCliente == 2) {
        cadastrarCliente(&venda->cliente);
    }

    int codigoProduto;
    printf("Digite o c�digo do produto: ");
    scanf("%d", &codigoProduto);

    int produtoEncontrado = 0;
    for (int i = 0; i < numProdutos; i++) {
        if (produtos[i].codigo == codigoProduto) {
            printf("Produto encontrado:\n");
            printf("Nome: %s\n", produtos[i].nome);
            printf("Pre�o: %.2f\n", produtos[i].preco);
            venda->valorTotal = produtos[i].preco;
            produtoEncontrado = 1;
            break;
        }
    }

    if (!produtoEncontrado) {
        printf("Produto n�o encontrado. Venda cancelada.\n");
        return;
    }

    int matriculaFuncionario;
    printf("Digite a matr�cula do funcion�rio: ");
    scanf("%d", &matriculaFuncionario);

    int funcionarioEncontrado = 0;
    for (int i = 0; i < numFuncionarios; i++) {
        if (funcionarios[i].matricula == matriculaFuncionario) {
            venda->funcionario = funcionarios[i];
            funcionarioEncontrado = 1;
            break;
        }
    }

    if (!funcionarioEncontrado) {
        printf("Funcion�rio n�o encontrado. Venda cancelada.\n");
        return;
    }

    venda->cliente.vendas += venda->valorTotal;
    venda->funcionario.comissao += 0.007 * venda->valorTotal;

    printf("Venda registrada com sucesso!\n");
}
void pesquisarProduto(struct Produto *produtos, int numProdutos) {
    int opcao;
    printf("Escolha a op��o de pesquisa (1 - C�digo, 2 - Pre�o): ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        int codigo;
        printf("Digite o c�digo do produto: ");
        scanf("%d", &codigo);

        for (int i = 0; i < numProdutos; i++) {
            if (produtos[i].codigo == codigo) {
                printf("Produto encontrado:\n");
                printf("Nome: %s\n", produtos[i].nome);
                printf("Pre�o: %.2f\n", produtos[i].preco);
                printf("C�digo: %d\n", produtos[i].codigo);
                return;
            }
        }

        printf("Produto n�o encontrado.\n");
    } else if (opcao == 2) {
        float preco;
        printf("Digite o pre�o do produto: ");
        scanf("%f", &preco);

        for (int i = 0; i < numProdutos; i++) {
            if (produtos[i].preco == preco) {
                printf("Produto encontrado:\n");
                printf("Nome: %s\n", produtos[i].nome);
                printf("Pre�o: %.2f\n", produtos[i].preco);
                printf("C�digo: %d\n", produtos[i].codigo);
                return;
            }
        }

        printf("Produto n�o encontrado.\n");
    } else {
        printf("Op��o inv�lida.\n");
    }
}

void pesquisarVenda(struct Cliente *clientes, int numClientes) {
    int opcao;
    printf("Escolha a op��o de pesquisa (1 - Data, 2 - Pre�o, 3 - Nome do cliente): ");
    scanf("%d", &opcao);

    if (opcao == 1) {
        // Implemente a pesquisa por data
    } else if (opcao == 2) {
        // Implemente a pesquisa por pre�o
    } else if (opcao == 3) {
        char nomeCliente[50];
        printf("Digite o nome do cliente: ");
        scanf("%s", nomeCliente);

        for (int i = 0; i < numClientes; i++) {
            if (strcmp(clientes[i].nome, nomeCliente) == 0) {
                printf("Venda encontrada:\n");
                printf("Nome do cliente: %s\n", clientes[i].nome);
                printf("Idade do cliente: %d\n", clientes[i].idade);
                printf("Vendas realizadas: %.2f\n", clientes[i].vendas);
                return;
            }
        }

        printf("Cliente n�o encontrado.\n");
    } else {
        printf("Op��o inv�lida.\n");
    }
}

void exibirProdutos(struct Produto *produtos, int numProdutos) {
    printf("Produtos cadastrados:\n");
    for (int i = 0; i < numProdutos; i++) {
        printf("Nome: %s\n", produtos[i].nome);
        printf("Pre�o: %.2f\n", produtos[i].preco);
        printf("C�digo: %d\n", produtos[i].codigo);
        printf("\n");
    }
}

void exibirVendas(struct Cliente *clientes, int numClientes) {
    printf("Vendas realizadas:\n");
    for (int i = 0; i < numClientes; i++) {
        printf("Nome do cliente: %s\n", clientes[i].nome);
        printf("Idade do cliente: %d\n", clientes[i].idade);
        printf("Vendas realizadas: %.2f\n", clientes[i].vendas);
        printf("\n");
    }
}


void calcularComissao(struct Funcionario *funcionarios, int numFuncionarios) {
    printf("C�lculo de comiss�o:\n");

    for (int i = 0; i < numFuncionarios; i++) {
        printf("Nome do funcion�rio: %s\n", funcionarios[i].nome);
        printf("Matr�cula: %d\n", funcionarios[i].matricula);
        printf("Comiss�o acumulada: %.2f\n", funcionarios[i].comissao);
        printf("Sal�rio base: 1300.00\n");
        printf("Sal�rio total: %.2f\n", 1300.00 + funcionarios[i].comissao);
        printf("\n");
    }
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;
    struct Produto produtos[100];
    int numProdutos = 0;

    struct Funcionario funcionarios[100];
    int numFuncionarios = 0;

    struct Cliente clientes[100];
    int numClientes = 0;

    struct Venda venda;

    // Carregar dados do arquivo ao iniciar o programa
    carregarProdutos(produtos, &numProdutos);
    carregarFuncionarios(funcionarios, &numFuncionarios);
    carregarClientes(clientes, &numClientes);

    do {
        printf("\nMenu:\n");
        printf("1 - Cadastrar produto\n");
        printf("2 - Cadastrar funcion�rio\n");
        printf("3 - Cadastrar cliente\n");
        printf("4 - Registrar venda\n");
        printf("5 - Pesquisar produto\n");
        printf("6 - Pesquisar venda\n");
        printf("7 - Exibir produtos cadastrados\n");
        printf("8 - Exibir vendas realizadas\n");
        printf("9 - Calcular comiss�o\n");
        printf("10 - Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);
        system("cls");

  switch (opcao) {
            case 1:
                cadastrarProduto(&produtos[numProdutos]);
                numProdutos++;
                break;
            case 2:
                cadastrarFuncionario(&funcionarios[numFuncionarios]);
                numFuncionarios++;
                break;
            case 3:
                cadastrarCliente(&clientes[numClientes]);
                numClientes++;
                break;
            case 4:
                registrarVenda(&venda, produtos, numProdutos, clientes, numClientes, funcionarios, numFuncionarios);
                break;
            case 5:
                pesquisarProduto(produtos, numProdutos);
                break;
            case 6:
                pesquisarVenda(clientes, numClientes);
                break;
            case 7:
                exibirProdutos(produtos, numProdutos);
                break;
            case 8:
                exibirVendas(clientes, numClientes);
                break;
            case 9:
                calcularComissao(funcionarios, numFuncionarios);
                break;
            case 10:
                // Salvar dados no arquivo ao sair do programa
                salvarProdutos(produtos, numProdutos);
                salvarFuncionarios(funcionarios, numFuncionarios);
                salvarClientes(clientes, numClientes);
                

                printf("Saindo do programa. Obrigado!\n");
                break;
            default:
                printf("Op��o inv�lida. Tente novamente.\n");
        }
    } while (opcao != 10);

    return 0;
}
