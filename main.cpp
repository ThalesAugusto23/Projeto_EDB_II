#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// 1. ÁRVORE AVL

struct NoAVL {
    int chave;
    int altura;
    NoAVL* esquerda;
    NoAVL* direita;

    NoAVL(int val) : chave(val), altura(1), esquerda(nullptr), direita(nullptr) {}
};

class ArvoreAVL {
private:
    NoAVL* raiz;
    int contadorRotacoes;

    int getAltura(NoAVL* n) {
        if (n == nullptr) return 0;
        return n->altura;
    }

    int getFatorBalanceamento(NoAVL* n) {
        if (n == nullptr) return 0;
        return getAltura(n->esquerda) - getAltura(n->direita);
    }

    void atualizarAltura(NoAVL* n) {
        if (n != nullptr) {
            n->altura = 1 + max(getAltura(n->esquerda), getAltura(n->direita));
        }
    }

    NoAVL* rotacionarDireita(NoAVL* y) {
        NoAVL* x = y->esquerda;
        NoAVL* T2 = x->direita;

        x->direita = y;
        y->esquerda = T2;

        atualizarAltura(y);
        atualizarAltura(x);

        contadorRotacoes++;
        return x;
    }

    NoAVL* rotacionarEsquerda(NoAVL* x) {
        NoAVL* y = x->direita;
        NoAVL* T2 = y->esquerda;

        y->esquerda = x;
        x->direita = T2;

        atualizarAltura(x);
        atualizarAltura(y);

        contadorRotacoes++;
        return y;
    }

    NoAVL* inserirRecursivo(NoAVL* no, int chave, bool &inserido) {
        if (no == nullptr) {
            inserido = true;
            return new NoAVL(chave);
        }

        if (chave < no->chave) {
            no->esquerda = inserirRecursivo(no->esquerda, chave, inserido);
        } else if (chave > no->chave) {
            no->direita = inserirRecursivo(no->direita, chave, inserido);
        } else {
            // Valor duplicado nao permitido
            inserido = false;
            return no;
        }

        atualizarAltura(no);
        int fb = getFatorBalanceamento(no);

        // Caso Esquerda-Esquerda
        if (fb > 1 && chave < no->esquerda->chave)
            return rotacionarDireita(no);

        // Caso Direita-Direita
        if (fb < -1 && chave > no->direita->chave)
            return rotacionarEsquerda(no);

        // Caso Esquerda-Direita
        if (fb > 1 && chave > no->esquerda->chave) {
            no->esquerda = rotacionarEsquerda(no->esquerda);
            return rotacionarDireita(no);
        }

        // Caso Direita-Esquerda
        if (fb < -1 && chave < no->direita->chave) {
            no->direita = rotacionarDireita(no->direita);
            return rotacionarEsquerda(no);
        }

        return no;
    }

    NoAVL* menorValorNo(NoAVL* no) {
        NoAVL* atual = no;
        while (atual->esquerda != nullptr)
            atual = atual->esquerda;
        return atual;
    }

    NoAVL* removerRecursivo(NoAVL* no, int chave, bool &removido) {
        if (no == nullptr) {
            removido = false;
            return nullptr;
        }

        if (chave < no->chave) {
            no->esquerda = removerRecursivo(no->esquerda, chave, removido);
        } else if (chave > no->chave) {
            no->direita = removerRecursivo(no->direita, chave, removido);
        } else {
            removido = true;
            // No com apenas um filho ou nenhum
            if ((no->esquerda == nullptr) || (no->direita == nullptr)) {
                NoAVL* temp = no->esquerda ? no->esquerda : no->direita;

                if (temp == nullptr) {
                    temp = no;
                    no = nullptr;
                } else {
                    *no = *temp;
                }
                delete temp;
            } else {
                // No com dois filhos: pega o sucessor em-ordem
                NoAVL* temp = menorValorNo(no->direita);
                no->chave = temp->chave;
                no->direita = removerRecursivo(no->direita, temp->chave, removido);
            }
        }

        if (no == nullptr) return nullptr;

        atualizarAltura(no);
        int fb = getFatorBalanceamento(no);

        // Balanceamento pos-remocao
        if (fb > 1 && getFatorBalanceamento(no->esquerda) >= 0)
            return rotacionarDireita(no);

        if (fb > 1 && getFatorBalanceamento(no->esquerda) < 0) {
            no->esquerda = rotacionarEsquerda(no->esquerda);
            return rotacionarDireita(no);
        }

        if (fb < -1 && getFatorBalanceamento(no->direita) <= 0)
            return rotacionarEsquerda(no);

        if (fb < -1 && getFatorBalanceamento(no->direita) > 0) {
            no->direita = rotacionarDireita(no->direita);
            return rotacionarEsquerda(no);
        }

        return no;
    }

    bool buscarRecursivo(NoAVL* no, int chave) {
        if (no == nullptr) return false;
        if (no->chave == chave) return true;
        if (chave < no->chave) return buscarRecursivo(no->esquerda, chave);
        return buscarRecursivo(no->direita, chave);
    }

    void emOrdemRecursivo(NoAVL* no) {
        if (no != nullptr) {
            emOrdemRecursivo(no->esquerda);
            cout << no->chave << " ";
            emOrdemRecursivo(no->direita);
        }
    }

    void exibirArvoreRecursivo(NoAVL* no, string prefixo, bool ehEsquerda) {
        if (no == nullptr) {
            return;
        }
    
        cout << prefixo;
    
        if (ehEsquerda) {
            cout << "|-- ";
        } else {
            cout << "\\-- ";
        }
    
        cout << no->chave << " (h=" << no->altura << ")" << endl;
    
        string novoPrefixo = prefixo + (ehEsquerda ? "|   " : "    ");
    
        exibirArvoreRecursivo(no->esquerda, novoPrefixo, true);
        exibirArvoreRecursivo(no->direita, novoPrefixo, false);
    }

public:
    ArvoreAVL() : raiz(nullptr), contadorRotacoes(0) {}

    void inserir(int chave) {
        bool inserido = false;
        raiz = inserirRecursivo(raiz, chave, inserido);
        if (inserido)
            cout << " [AVL] Encomenda ID " << chave << " inserida com sucesso." << endl;
        else
            cout << " [Aviso] ID " << chave << " ja existe na arvore!" << endl;
    }

    void remover(int chave) {
        bool removido = false;
        raiz = removerRecursivo(raiz, chave, removido);
        if (removido)
            cout << " [AVL] Encomenda ID " << chave << " removida com sucesso." << endl;
        else
            cout << " [Aviso] ID " << chave << " nao foi encontrado para remocao!" << endl;
    }

    bool buscar(int chave) {
        return buscarRecursivo(raiz, chave);
    }

    void emOrdem() {
        if (raiz == nullptr) {
            cout << "Arvore vazia." << endl;
            return;
        }
        emOrdemRecursivo(raiz);
        cout << endl;
    }

    void exibir() {
        if (raiz == nullptr) {
            cout << "Arvore vazia." << endl;
            return;
        }
        exibirArvoreRecursivo(raiz, "", false);
    }

    int getAlturaTotal() {
        return getAltura(raiz);
    }

    int getRotacoes() {
        return contadorRotacoes;
    }
};

// 2. ÁRVORE RUBRO-NEGRA

enum Cor { VERMELHO, PRETO };

struct NoRN {
    int chave;
    Cor cor;
    NoRN *esquerda, *direita, *pai;

    NoRN(int val) : chave(val), cor(VERMELHO), esquerda(nullptr), direita(nullptr), pai(nullptr) {}
};

class ArvoreRubroNegra {
private:
    NoRN* raiz;
    NoRN* NULO; // No sentinela representando folhas pretas
    int contadorRotacoes;

    void rotacionarEsquerda(NoRN* x) {
        NoRN* y = x->direita;
        x->direita = y->esquerda;
        if (y->esquerda != NULO) {
            y->esquerda->pai = x;
        }
        y->pai = x->pai;
        if (x->pai == nullptr) {
            raiz = y;
        } else if (x == x->pai->esquerda) {
            x->pai->esquerda = y;
        } else {
            x->pai->direita = y;
        }
        y->esquerda = x;
        x->pai = y;
        contadorRotacoes++;
    }

    void rotacionarDireita(NoRN* x) {
        NoRN* y = x->esquerda;
        x->esquerda = y->direita;
        if (y->direita != NULO) {
            y->direita->pai = x;
        }
        y->pai = x->pai;
        if (x->pai == nullptr) {
            raiz = y;
        } else if (x == x->pai->direita) {
            x->pai->direita = y;
        } else {
            x->pai->esquerda = y;
        }
        y->direita = x;
        x->pai = y;
        contadorRotacoes++;
    }

    void ajustarInsercao(NoRN* k) {
        NoRN* tio;
        while (k->pai && k->pai->cor == VERMELHO) {
            if (k->pai == k->pai->pai->esquerda) {
                tio = k->pai->pai->direita;
                if (tio->cor == VERMELHO) {
                    k->pai->cor = PRETO;
                    tio->cor = PRETO;
                    k->pai->pai->cor = VERMELHO;
                    k = k->pai->pai;
                } else {
                    if (k == k->pai->direita) {
                        k = k->pai;
                        rotacionarEsquerda(k);
                    }
                    k->pai->cor = PRETO;
                    k->pai->pai->cor = VERMELHO;
                    rotacionarDireita(k->pai->pai);
                }
            } else {
                tio = k->pai->pai->esquerda;
                if (tio->cor == VERMELHO) {
                    k->pai->cor = PRETO;
                    tio->cor = PRETO;
                    k->pai->pai->cor = VERMELHO;
                    k = k->pai->pai;
                } else {
                    if (k == k->pai->esquerda) {
                        k = k->pai;
                        rotacionarDireita(k);
                    }
                    k->pai->cor = PRETO;
                    k->pai->pai->cor = VERMELHO;
                    rotacionarEsquerda(k->pai->pai);
                }
            }
            if (k == raiz) break;
        }
        raiz->cor = PRETO;
    }

    void transplantar(NoRN* u, NoRN* v) {
        if (u->pai == nullptr) {
            raiz = v;
        } else if (u == u->pai->esquerda) {
            u->pai->esquerda = v;
        } else {
            u->pai->direita = v;
        }
        v->pai = u->pai;
    }

    void ajustarRemocao(NoRN* x) {
        NoRN* w;
        while (x != raiz && x->cor == PRETO) {
            if (x == x->pai->esquerda) {
                w = x->pai->direita;
                if (w->cor == VERMELHO) {
                    w->cor = PRETO;
                    x->pai->cor = VERMELHO;
                    rotacionarEsquerda(x->pai);
                    w = x->pai->direita;
                }
                if (w->esquerda->cor == PRETO && w->direita->cor == PRETO) {
                    w->cor = VERMELHO;
                    x = x->pai;
                } else {
                    if (w->direita->cor == PRETO) {
                        w->esquerda->cor = PRETO;
                        w->cor = VERMELHO;
                        rotacionarDireita(w);
                        w = x->pai->direita;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = PRETO;
                    w->direita->cor = PRETO;
                    rotacionarEsquerda(x->pai);
                    x = raiz;
                }
            } else {
                w = x->pai->esquerda;
                if (w->cor == VERMELHO) {
                    w->cor = PRETO;
                    x->pai->cor = VERMELHO;
                    rotacionarDireita(x->pai);
                    w = x->pai->esquerda;
                }
                if (w->direita->cor == PRETO && w->esquerda->cor == PRETO) {
                    w->cor = VERMELHO;
                    x = x->pai;
                } else {
                    if (w->esquerda->cor == PRETO) {
                        w->direita->cor = PRETO;
                        w->cor = VERMELHO;
                        rotacionarEsquerda(w);
                        w = x->pai->esquerda;
                    }
                    w->cor = x->pai->cor;
                    x->pai->cor = PRETO;
                    w->esquerda->cor = PRETO;
                    rotacionarDireita(x->pai);
                    x = raiz;
                }
            }
        }
        x->cor = PRETO;
    }

    NoRN* menorValorNo(NoRN* no) {
        while (no->esquerda != NULO) {
            no = no->esquerda;
        }
        return no;
    }

    void emOrdemRecursivo(NoRN* no) {
        if (no != NULO) {
            emOrdemRecursivo(no->esquerda);
            cout << no->chave << " ";
            emOrdemRecursivo(no->direita);
        }
    }

    void exibirArvoreRecursivo(NoRN* no, string prefixo, bool ehEsquerda) {
        if (no == NULO) {
            return;
        }
    
        cout << prefixo;
    
        if (ehEsquerda) {
            cout << "|-- ";
        } else {
            cout << "\\-- ";
        }
    
        cout << no->chave
             << " [" << (no->cor == VERMELHO ? "Vermelho" : "Preto") << "]"
             << endl;
    
        string novoPrefixo = prefixo + (ehEsquerda ? "|   " : "    ");
    
        exibirArvoreRecursivo(no->esquerda, novoPrefixo, true);
        exibirArvoreRecursivo(no->direita, novoPrefixo, false);
    }

    int calcularAlturaRecursivo(NoRN* no) {
        if (no == NULO) return 0;
        return 1 + max(calcularAlturaRecursivo(no->esquerda), calcularAlturaRecursivo(no->direita));
    }

public:
    ArvoreRubroNegra() : contadorRotacoes(0) {
        NULO = new NoRN(0);
        NULO->cor = PRETO;
        NULO->esquerda = nullptr;
        NULO->direita = nullptr;
        raiz = NULO;
    }

    void inserir(int chave) {
        NoRN* no = new NoRN(chave);
        no->pai = nullptr;
        no->chave = chave;
        no->esquerda = NULO;
        no->direita = NULO;
        no->cor = VERMELHO;

        NoRN* y = nullptr;
        NoRN* x = raiz;

        while (x != NULO) {
            y = x;
            if (no->chave < x->chave) {
                x = x->esquerda;
            } else if (no->chave > x->chave) {
                x = x->direita;
            } else {
                cout << " [Aviso] ID " << chave << " ja existe na arvore RN!" << endl;
                delete no;
                return;
            }
        }

        no->pai = y;
        if (y == nullptr) {
            raiz = no;
        } else if (no->chave < y->chave) {
            y->esquerda = no;
        } else {
            y->direita = no;
        }

        if (no->pai == nullptr) {
            no->cor = PRETO;
            cout << " [RN] Encomenda ID " << chave << " inserida com sucesso." << endl;
            return;
        }

        if (no->pai->pai == nullptr) {
            cout << " [RN] Encomenda ID " << chave << " inserida com sucesso." << endl;
            return;
        }

        ajustarInsercao(no);
        cout << " [RN] Encomenda ID " << chave << " inserida com sucesso." << endl;
    }

    void remover(int chave) {
        NoRN* z = raiz;
        while (z != NULO) {
            if (z->chave == chave) break;
            if (chave < z->chave) z = z->esquerda;
            else z = z->direita;
        }

        if (z == NULO) {
            cout << " [Aviso] ID " << chave << " nao foi encontrado para remocao!" << endl;
            return;
        }

        NoRN* y = z;
        NoRN* x;
        Cor corOriginalY = y->cor;

        if (z->esquerda == NULO) {
            x = z->direita;
            transplantar(z, z->direita);
        } else if (z->direita == NULO) {
            x = z->esquerda;
            transplantar(z, z->esquerda);
        } else {
            y = menorValorNo(z->direita);
            corOriginalY = y->cor;
            x = y->direita;

            if (y->pai == z) {
                x->pai = y;
            } else {
                transplantar(y, y->direita);
                y->direita = z->direita;
                y->direita->pai = y;
            }

            transplantar(z, y);
            y->esquerda = z->esquerda;
            y->esquerda->pai = y;
            y->cor = z->cor;
        }

        delete z;
        if (corOriginalY == PRETO) {
            ajustarRemocao(x);
        }
        cout << " [RN] Encomenda ID " << chave << " removida com sucesso." << endl;
    }

    bool buscar(int chave) {
        NoRN* atual = raiz;
        while (atual != NULO) {
            if (chave == atual->chave) return true;
            if (chave < atual->chave) atual = atual->esquerda;
            else atual = atual->direita;
        }
        return false;
    }

    void emOrdem() {
        if (raiz == NULO) {
            cout << "Arvore vazia." << endl;
            return;
        }
        emOrdemRecursivo(raiz);
        cout << endl;
    }

    void exibir() {
        if (raiz == NULO) {
            cout << "Arvore vazia." << endl;
            return;
        }
        exibirArvoreRecursivo(raiz, "", false);
    }

    int getAlturaTotal() {
        return calcularAlturaRecursivo(raiz);
    }

    int getRotacoes() {
        return contadorRotacoes;
    }
};

// 3. MENU INTERATIVO

void menuOperacoesAVL(ArvoreAVL &arvore) {
    int opcao, id;
    do {
        cout << "\n--- MENU ARVORE AVL ---" << endl;
        cout << "1. Inserir encomenda (ID)" << endl;
        cout << "2. Remover encomenda (ID)" << endl;
        cout << "3. Buscar encomenda (ID)" << endl;
        cout << "4. Percurso Em-Ordem" << endl;
        cout << "5. Exibir estrutura da Arvore" << endl;
        cout << "6. Estatisticas (Altura e Rotacoes)" << endl;
        cout << "0. Voltar ao menu principal" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "Digite o ID para insercao: ";
                cin >> id;
                arvore.inserir(id);
                break;
            case 2:
                cout << "Digite o ID para remocao: ";
                cin >> id;
                arvore.remover(id);
                break;
            case 3:
                cout << "Digite o ID para busca: ";
                cin >> id;
                if (arvore.buscar(id))
                    cout << " [Resultado] Encomenda ID " << id << " encontrada!" << endl;
                else
                    cout << " [Resultado] Encomenda ID " << id << " NAO foi encontrada." << endl;
                break;
            case 4:
                cout << "Elementos Em-Ordem: ";
                arvore.emOrdem();
                break;
            case 5:
                cout << "\nEstrutura atual da AVL (chave e altura):" << endl;
                arvore.exibir();
                break;
            case 6:
                cout << "Altura atual: " << arvore.getAlturaTotal() << endl;
                cout << "Total de rotacoes acumuladas: " << arvore.getRotacoes() << endl;
                break;
            case 0:
                cout << "Saindo do menu AVL..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);
}

void menuOperacoesRN(ArvoreRubroNegra &arvore) {
    int opcao, id;
    do {
        cout << "\n--- MENU ARVORE RUBRO-NEGRA ---" << endl;
        cout << "1. Inserir encomenda (ID)" << endl;
        cout << "2. Remover encomenda (ID)" << endl;
        cout << "3. Buscar encomenda (ID)" << endl;
        cout << "4. Percurso Em-Ordem" << endl;
        cout << "5. Exibir estrutura da Arvore [V=Vermelho, P=Preto]" << endl;
        cout << "6. Estatisticas (Altura e Rotacoes)" << endl;
        cout << "0. Voltar ao menu principal" << endl;
        cout << "Escolha uma opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                cout << "Digite o ID para insercao: ";
                cin >> id;
                arvore.inserir(id);
                break;
            case 2:
                cout << "Digite o ID para remocao: ";
                cin >> id;
                arvore.remover(id);
                break;
            case 3:
                cout << "Digite o ID para busca: ";
                cin >> id;
                if (arvore.buscar(id))
                    cout << " [Resultado] Encomenda ID " << id << " encontrada!" << endl;
                else
                    cout << " [Resultado] Encomenda ID " << id << " NAO foi encontrada." << endl;
                break;
            case 4:
                cout << "Elementos Em-Ordem: ";
                arvore.emOrdem();
                break;
            case 5:
                cout << "\nEstrutura atual da Rubro-Negra:" << endl;
                arvore.exibir();
                break;
            case 6:
                cout << "Altura atual: " << arvore.getAlturaTotal() << endl;
                cout << "Total de rotacoes acumuladas: " << arvore.getRotacoes() << endl;
                break;
            case 0:
                cout << "Saindo do menu Rubro-Negra..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);
}

int main() {
    ArvoreAVL avl;
    ArvoreRubroNegra rn;
    int opcao;

    cout << "=================================================" << endl;
    cout << "  AMAZON - BLACK FRIDAY   " << endl;
    cout << "=================================================" << endl;

    do {
        cout << "\nEscolha a estrutura de dados que deseja manipular:" << endl;
        cout << "1. Arvore AVL" << endl;
        cout << "2. Arvore Rubro-Negra" << endl;
        cout << "0. Sair do programa" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        switch (opcao) {
            case 1:
                menuOperacoesAVL(avl);
                break;
            case 2:
                menuOperacoesRN(rn);
                break;
            case 0:
                cout << "Encerrando o sistema..." << endl;
                break;
            default:
                cout << "Opcao invalida!" << endl;
        }
    } while (opcao != 0);

    return 0;
}