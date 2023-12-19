
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

typedef struct {
    int codigo;
    char nome[30];
    float comissao;
} Vendedor;

Vendedor vendedores[50];

static int contador_vendedor = 0;

typedef struct {
    int codigo;
    char nome[30];
    float preco;
} Produto;

typedef struct {
    Produto produto;
    int quantidade;
} Carrinho;

void infoProduto(Produto prod);
void menu();
void cadastrarProduto();
void listarProdutos();
void comprarProduto();
void visualizarCarrinho();
Produto pegarProdutoPorCodigo(int codigo);
int *temNoCarrinho(int codigo);
void fecharPedido();
void carregarProdutos();
void salvarCarrinho();
void carregarCarrinho();
void encerrarPrograma();
void registrarVendedor();
void cadastrarComissao();
void calcularComissao();
void carregarVendedores();
void salvarVendedores();
void mostrarVendedores();


static int contador_produto = 0;
static int contador_carrinho = 0;
static Carrinho carrinho[50];
static Produto produtos[50];

int main() {
    setlocale(LC_ALL, "Portuguese");

    // Adicionar chamada para carregar produtos
    carregarProdutos();

    menu();
    return 0;
}

void infoProduto(Produto prod) {
    printf("Código: %d \nNome: %s \nPreço: %.2f\n", prod.codigo, strtok(prod.nome, "\n"), prod.preco);
}

void menu() {
    printf("=========================================\n");
    printf("================ Bem-vindo(a) ===========\n");
    printf("================  Geek Shop   ===========\n");
    printf("=========================================\n");

    printf("Selecione uma opção abaixo: \n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Listar produtos\n");
    printf("3 - Comprar produto\n");
    printf("4 - Visualizar carrinho\n");
    printf("5 - Fechar pedido\n");
    printf("6 - Registrar vendedor\n");
    printf("7 - Cadastrar comissão\n");
    printf("8 - Calcular comissão\n");
    printf("9 - Mostrar vendedores\n");
    printf("10 - Sair do sistema\n");

    int opcao;
    scanf("%d", &opcao);
    getchar();

    switch (opcao) {
        case 1:
            cadastrarProduto();
            break;
        case 2:
            listarProdutos();
            break;
        case 3:
            comprarProduto();
            break;
        case 4:
            visualizarCarrinho();
            break;
        case 5:
            fecharPedido();
            break;
        case 6:
            registrarVendedor();
            break;
        case 7:
            cadastrarComissao();
            break;
        case 8:
            calcularComissao();
            break;
        case 9:
            mostrarVendedores();
            break;
        case 10:
            encerrarPrograma();
        default:
            printf("Opção inválida.\n");
            menu();
            break;
    }
}

void cadastrarProduto() {
    printf("Cadastro de Produto\n");
    printf("====================\n");

    printf("Informe o nome do produto: \n");
    fgets(produtos[contador_produto].nome, 30, stdin);

    printf("Informe o preço do produto: \n");
    scanf("%f", &produtos[contador_produto].preco);

    printf("O produto %s foi cadastrado com sucesso.\n", strtok(produtos[contador_produto].nome, "\n"));

    produtos[contador_produto].codigo = (contador_produto + 1);
    contador_produto++;

    menu();
}

void listarProdutos() {
    if (contador_produto > 0) {
        printf("Listagem de produtos.\n");
        printf("---------------------\n");
        for (int i = 0; i < contador_produto; i++) {
            infoProduto(produtos[i]);
            printf("------------------\n");
        }
    } else {
        printf("Não temos ainda produtos cadastrados.\n");
    }

    menu();
}

void comprarProduto() {
    if (contador_produto > 0) {
        printf("Informe o código do produto que deseja adicionar ao carrinho.\n");

        printf("========== Produtos Disponíveis ===========\n");
        for (int i = 0; i < contador_produto; i++) {
            infoProduto(produtos[i]);
            printf("---------------------\n");
        }
        int codigo;
        scanf("%d", &codigo);
        getchar();

        int tem_mercado = 0;
        for (int i = 0; i < contador_produto; i++) {
            if (produtos[i].codigo == codigo) {
                tem_mercado = 1;

                if (contador_carrinho > 0) {
                    int *retorno = temNoCarrinho(codigo);

                    if (retorno[0] == 1) {
                        carrinho[retorno[1]].quantidade++;
                        printf("Aumentei a quantidade do produto %s já existente no carrinho.\n",
                               strtok(carrinho[retorno[1]].produto.nome, "\n"));
                    } else {
                        Produto p = pegarProdutoPorCodigo(codigo);
                        carrinho[contador_carrinho].produto = p;
                        carrinho[contador_carrinho].quantidade = 1;
                        contador_carrinho++;
                        printf("O produto %s foi adicionado ao carrinho.\n", strtok(p.nome, "\n"));
                    }
                } else {
                    Produto p = pegarProdutoPorCodigo(codigo);
                    carrinho[contador_carrinho].produto = p;
                    carrinho[contador_carrinho].quantidade = 1;
                    contador_carrinho++;
                    printf("O produto %s foi adicionado ao carrinho.\n", strtok(p.nome, "\n"));
                }
            }
        }
        if (tem_mercado < 1) {
            printf("Não foi encontrado o produto com código %d.\n", codigo);
        }
    } else {
        printf("Ainda não existem produtos para vender.\n");
    }

    menu();
}

void visualizarCarrinho() {
    if (contador_carrinho > 0) {
        printf("Produtos do Carrinho\n");
        printf("--------------------\n");
        for (int i = 0; i < contador_carrinho; i++) {
            infoProduto(carrinho[i].produto);
            printf("Quantidade: %d\n", carrinho[i].quantidade);
            printf("-----------------\n");
        }
    } else {
        printf("Não temos ainda produtos no carrinho.\n");
    }

    menu();
}

Produto pegarProdutoPorCodigo(int codigo) {
    Produto p;
    for (int i = 0; i < contador_produto; i++) {
        if (produtos[i].codigo == codigo) {
            p = produtos[i];
        }
    }
    return p;
}

int *temNoCarrinho(int codigo) {
    static int retorno[] = {0, 0};
    for (int i = 0; i < contador_carrinho; i++) {
        if (carrinho[i].produto.codigo == codigo) {
            retorno[0] = 1; // tem o produto com este código no carrinho
            retorno[1] = i; // o índice do produto no carrinho
        }
    }
    return retorno;
}

void fecharPedido() {
    if (contador_carrinho > 0) {
        float valorTotal = 0.0;
        printf("Produtos do Carrinho\n");
        printf("--------------------\n");
        for (int i = 0; i < contador_carrinho; i++) {
            Produto p = carrinho[i].produto;
            int quantidade = carrinho[i].quantidade;
            valorTotal += p.preco * quantidade;
            infoProduto(p);
            printf("Quantidade: %d\n", quantidade);
            printf("---------------\n");
        }
        printf("Sua fatura é R$ %.2f\n", valorTotal);

        // limpar carrinho
        contador_carrinho = 0;
        printf("Obrigado pela preferência.\n");
    } else {
        printf("Você não tem nenhum produto no carrinho ainda.\n");
    }

    menu();
}

void salvarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos para escrita.\n");
        return;
    }

    for (int i = 0; i < contador_produto; i++) {
        fprintf(arquivo, "%d %s %.2f\n", produtos[i].codigo, strtok(produtos[i].nome, "\n"), produtos[i].preco);
    }

    fclose(arquivo);
}

void carregarProdutos() {
    FILE *arquivo = fopen("produtos.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos para leitura.\n");
        return;
    }

    while (fscanf(arquivo, "%d %[^\n] %f\n", &produtos[contador_produto].codigo, produtos[contador_produto].nome, &produtos[contador_produto].preco) == 3) {
        contador_produto++;
    }

    fclose(arquivo);
}

void salvarCarrinho() {
    FILE *arquivo = fopen("carrinho.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo do carrinho para escrita.\n");
        return;
    }

    for (int i = 0; i < contador_carrinho; i++) {
        fprintf(arquivo, "%d %s %.2f %d\n", carrinho[i].produto.codigo, strtok(carrinho[i].produto.nome, "\n"), carrinho[i].produto.preco, carrinho[i].quantidade);
    }

    fclose(arquivo);
}

void carregarCarrinho() {
    FILE *arquivo = fopen("carrinho.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo do carrinho para leitura.\n");
        return;
    }

    while (fscanf(arquivo, "%d %[^\n] %f %d\n", &carrinho[contador_carrinho].produto.codigo, carrinho[contador_carrinho].produto.nome, &carrinho[contador_carrinho].produto.preco, &carrinho[contador_carrinho].quantidade) == 4) {
        contador_carrinho++;
    }

    fclose(arquivo);
}

void registrarVendedor() {
    printf("Registro de Vendedor\n");
    printf("====================\n");

    printf("Informe o nome do vendedor: \n");
    fgets(vendedores[contador_vendedor].nome, 30, stdin);

    printf("Informe o código do vendedor: \n");
    scanf("%d", &vendedores[contador_vendedor].codigo);

    printf("O vendedor %s foi registrado com sucesso.\n", strtok(vendedores[contador_vendedor].nome, "\n"));

    contador_vendedor++;

    menu();
}

void cadastrarComissao() {
    printf("Cadastro de Comissão\n");
    printf("====================\n");

    printf("Informe o código do vendedor: \n");
    int codigoVendedor;
    scanf("%d", &codigoVendedor);

    int vendedorEncontrado = 0;
    for (int i = 0; i < contador_vendedor; i++) {
        if (vendedores[i].codigo == codigoVendedor) {
            vendedorEncontrado = 1;

            printf("Informe o valor da venda: \n");
            float valorVenda;
            scanf("%f", &valorVenda);

            vendedores[i].comissao += valorVenda * 0.1;

            printf("Comissão cadastrada com sucesso para o vendedor %s.\n", strtok(vendedores[i].nome, "\n"));
            menu();
        }
    }

    if (vendedorEncontrado == 0) {
        printf("Não foi encontrado um vendedor com o código %d.\n", codigoVendedor);
    }

    menu();
}

void calcularComissao() {
    printf("Cálculo de Comissão\n");
    printf("===================\n");

    printf("Informe o código do vendedor: \n");
    int codigoVendedor;
    scanf("%d", &codigoVendedor);

    int vendedorEncontrado = 0;
    for (int i = 0; i < contador_vendedor; i++) {
        if (vendedores[i].codigo == codigoVendedor) {
            vendedorEncontrado = 1;

            printf("A comissão do vendedor %s é de R$ %.2f.\n", strtok(vendedores[i].nome, "\n"), vendedores[i].comissao);
        }
    }

    if (vendedorEncontrado == 0) {
        printf("Não foi encontrado um vendedor com o código %d.\n", codigoVendedor);
    }

    menu();
}

void carregarVendedores() {
    FILE *arquivo = fopen("vendedores.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendedores para leitura.\n");
        return;
    }

    contador_vendedor = 0;  // Inicializa o contador de vendedores

    while (fscanf(arquivo, "%d %29[^\n] %f\n", &vendedores[contador_vendedor].codigo, vendedores[contador_vendedor].nome, &vendedores[contador_vendedor].comissao) == 3) {
        contador_vendedor++;
        if (contador_vendedor >= 50) {
            printf("Limite de vendedores excedido. Ajuste o código para acomodar mais vendedores.\n");
            break;
        }
    }

    fclose(arquivo);
}

void salvarVendedores() {
    FILE *arquivo = fopen("vendedores.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendedores para escrita.\n");
        return;
    }

    for (int i = 0; i < contador_vendedor; i++) {
        fprintf(arquivo, "%d %s %.2f\n", vendedores[i].codigo, strtok(vendedores[i].nome, "\n"), vendedores[i].comissao);
    }

    fclose(arquivo);
}
void mostrarVendedores() {
    if (contador_vendedor > 0) {
        printf("Lista de Vendedores\n");
        printf("--------------------\n");
        for (int i = 0; i < contador_vendedor; i++) {
            printf("Código: %d\n", vendedores[i].codigo);
            printf("Nome: %s\n", vendedores[i].nome);
            printf("Comissão: R$ %.2f\n", vendedores[i].comissao);
            printf("--------------------\n");
            sleep(1);
        }
        sleep(2);
    } else {
        printf("Não há vendedores registrados.\n");
        sleep(2);
    }
}

void encerrarPrograma() {
    salvarProdutos();
    salvarCarrinho();
    salvarVendedores();
    printf("Saindo do programa. Obrigado!\n");
    exit(0);
}
