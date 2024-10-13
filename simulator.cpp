#include "simulator.hpp"
#include "logger.hpp"
#include "printer.hpp"
#include "spooler.hpp"
#include <iostream>
#include <charconv>

std::expected<unsigned int, std::string>
Simulator::parseArg(const std::string_view arg_name, const std::optional<std::string_view> arg_value,
                    const int default_value) {
    int value = default_value;
    if (arg_value.has_value()) {
        auto [ptr, ec] = std::from_chars(arg_value->data(), arg_value->data() + arg_value->size(), value);
        if (ec != std::errc{} || value < 1) {
            return std::unexpected(std::format("Invalid argument value: <{}> = '{}'", arg_name, *arg_value));
        }
    }
    return value;
}

std::expected<std::pair<unsigned int, unsigned int>, std::string>
Simulator::getArgs(int argc, char *argv[]) {
    auto num_process = parseArg("num_process",
                                argc > 1 ? std::optional(argv[1]) : std::nullopt,
                                DEFAULT_NUMBER_PROCESS);
    auto num_printers = parseArg("num_printers",
                                 argc > 2 ? std::optional(argv[2]) : std::nullopt,
                                 DEFAULT_NUMBER_PRINTERS);

    if (num_process && num_printers) {
        return std::make_pair(*num_process, *num_printers);
    }
    return std::unexpected(num_process ? num_printers.error() : num_process.error());
}

void Simulator::start(int argc, char *argv[]) {
    auto args = getArgs(argc, argv);
    if (!args) {
        std::cerr << "[Error] " << args.error() << std::endl
                  << "Usage: print-spooler <num_process> <num_printers>" << std::endl
                  << "  <num_process>:  Number of processes (positive integer)" << std::endl
                  << "  <num_printers>: Number of printers (positive integer)" << std::endl
                  << "Example: 'print-spooler 4 2'" << std::endl
                  << "- Please check your input and try again." << std::endl;
        exit(EXIT_FAILURE);
    }

    auto [num_process, num_printers] = *args;
    start(num_process, num_printers);
}

void Simulator::start(unsigned int num_process, unsigned int num_printers) {
    logger::print("Starting printer spooler simulation with {} process and {} printers...\n",
                  num_process, num_printers);

    Spooler spooler;
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (unsigned int id = 1; id <= num_printers; id++) {
        spooler.addPrinter(Printer(std::format("Impressora {}", id)));
    }
    spooler.start();

//    for (int pid = 1; pid <= num_process; pid++) {
//        process.emplace_back(pid);
//    }
//    producers.reserve(process.size());
//    for (auto &p : process) {
//        producers.emplace_back(p.start(spooler));
//    }

    for (int pid = 1; pid <= num_process; pid++) {
        producers.emplace_back([pid, &spooler]() {
            Process p(pid);
            p.start(spooler).join();
        });
    }

    for (auto &t: producers) {
        if (t.joinable()) {
            t.join();
        }
    }
    logger::log("PARANDO O POOL!");
    spooler.stop();
}

std::thread Process::start(Spooler &spooler) {
    return std::thread([this, &spooler] {
        int n = generateRandomNumber(5, 10);
        logger::print("Processo [{}] enviará {} documentos\n", pid, n);
        for (int i = 0; i < n; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomNumber(0, 5000)));
            auto [file, num_pages] = getRandomFile();
            spooler.print(pid, file, num_pages,
                          *PrintRequest::priorityOf(generateRandomNumber(1, 5)));
        }
    });
}

void Process::stop() {
    Logger::get().debug("Parando process {}\n", pid);
}

std::pair<std::string_view, unsigned int> Process::getRandomFile() {
    static std::vector<std::pair<std::string_view, int>> files = {{
          {"anotacoes.txt", 1},
          {"apresentacao_final.pptx", 50},
          {"apresentacao.pptx", 40},
          {"artigo_academico.pdf", 16},
          {"ata_reuniao.docx", 2},
          {"backup_db.sql", 1},
          {"balanco.xlsx", 10},
          {"codigo_aula.cpp", 1},
          {"comprovante.txt", 1},
          {"config_sys.ini", 1},
          {"config_servidor.conf", 2},
          {"configuracao.yaml", 1},
          {"curriculo.pdf", 3},
          {"dados_2024.xlsx", 3},
          {"dicionario_termos.json", 1},
          {"diagrama_fluxo.png", 1},
          {"documentacao_tecnica.pdf", 3},
          {"documento.pdf", 1},
          {"documento_referencia.odt", 1},
          {"documento_reuniao.rtf", 2},
          {"ficha_cadastro.pdf", 3},
          {"figurinha.gif", 1},
          {"foto.jpg", 1},
          {"grafico_vendas.svg", 1},
          {"imagem_fundo.png", 1},
          {"livro.docx", 1},
          {"lista_compras.md", 1},
          {"log_sistema.log", 60},
          {"manual_usuario.pdf", 30},
          {"modelo_pesquisa.tex", 5},
          {"modelo_tese.docx", 18},
          {"notas.txt", 1},
          {"planilha_financeira.ods", 1},
          {"plano_trabalho.docx", 3},
          {"projeto.cpp", 1},
          {"projeto_website.html", 2},
          {"proposta_comercial.docx", 7},
          {"receita_culinaria.txt", 1},
          {"relatorio_analise.docx", 25},
          {"relatorio_execucao.xls", 14},
          {"relatorio_finalizado.pdf", 32},
          {"relatorio_de_vendas.xlsx", 22},
          {"resumo_aula.docx", 7},
          {"roteiro.pdf", 10},
          {"slides_trabalho.odp", 25},
          {"tabela_info.csv", 3},
          {"tabela_preco.tsv", 2},
          {"template_email.eml", 1},
          {"trabalho_final.docx", 45},
          {"tutorial.docx", 9}
    }};
    return files[generateRandomNumber(0, static_cast<int>(files.size()))];
}
