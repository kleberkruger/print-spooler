#include "simulator.hpp"

/**
 * Inicializa a aplicação
 *
 * @param argc quantidade de argumentos
 * @param argv valores dos argumentos
 *
 * @return 0 caso a aplicação executou sem erros
 */
int main(int argc, char *argv[]) {

    Simulator::start(argc, argv);

    return EXIT_SUCCESS;
}