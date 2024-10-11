#pragma once

#include <csignal>
#include "spooler.hpp"

class Process {
public:

    explicit Process(pid_t pid) : pid(pid) {}

    std::thread startPrintOrders(Spooler &spooler) const;

private:
    pid_t pid;

    static auto getRandomFile() {
        return FILES[generateRandomNumber(0, FILES.size())];
    }

    static constexpr std::array<std::pair<std::string_view, int>, 50> FILES = {{
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
};

//// Arquivos de texto geralmente com poucas páginas
//".txt", ".md", ".ini", ".yaml", ".properties", ".cfg", ".plist",
//".c", ".cc", ".cpp", ".h", "hpp", ".java", ".kotlin", ".kt", ".py", ".dart", ".xml", ".xaml", ".json",
//".html", ".css", ".less", ".scss", ".js", ".ts", ".php", ".rb", ".go", ".swift", ".sh", ".r", ".lua",
//".sql", ".schema",
//
//// Arquivos de texto geralmente com tamanho variado de páginas
//".pdf", ".tex", ".doc", ".docx", ".csv", ".xls", ".xlsx", ".ppt", ".pptx", ".rtf", ".odt", ".ods",
//".odp", ".log",
//
//// Imagens possuem geralmente
//".jpg", ".jpeg", ".png", ".gif", ".bmp", ".svg", ".ico"

class Simulator {
public:

    static void
    start(unsigned int num_process, unsigned int num_printers);
};
