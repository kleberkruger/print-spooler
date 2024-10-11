#pragma once

#include <csignal>
#include "spooler.hpp"

class Process {
public:

    explicit Process(pid_t pid) : pid(pid) {}

    std::thread startPrintOrders(Spooler &spooler) const;

private:
    pid_t pid;

    static std::string_view getRandomFile() {
        return FILES[generateRandomNumber(0, FILES.size())];
    }

    static constexpr std::array<std::string_view, 50> FILES = {
            "anotacoes.txt",
            "apresentacao_final.pptx",
            "apresentacao.pptx",
            "artigo_academico.pdf",
            "ata_reuniao.txt",
            "backup_db.sql",
            "backup.tar.gz",
            "curriculo.pdf",
            "config_sys.ini",
            "config_servidor.conf",
            "configuracao.yaml",
            "dados_2024.xlsx",
            "dicionario_termos.json",
            "diagrama_fluxo.vsdx",
            "documentacao_tecnica.pdf",
            "documento.pdf",
            "documento_aprovacao.txt",
            "documento_referencia.odt",
            "documento_reuniao.rtf",
            "ficha_cadastro.pdf",
            "foto.jpg",
            "grafico_vendas.svg",
            "imagem_capitulo.gif",
            "imagem_fundo.png",
            "imagem_projeto.bmp",
            "lista_compras.md",
            "logs_sistema.log",
            "manual_usuario.pdf",
            "modelo_pesquisa.tex",
            "modelo_template.docx",
            "notas.txt",
            "planilha_financeira.ods",
            "plano_trabalho.docx",
            "projeto.cpp",
            "projeto_website.html",
            "proposta_comercial.docx",
            "receita_culinaria.txt",
            "relatorio_analise.docx",
            "relatorio_execucao.xls",
            "relatorio_finalizado.pdf",
            "relatorio_de_vendas.xlsx",
            "resumo_aula.docx"
            "roteiro.pdf"
            "slide.odp",
            "tabela_info.csv",
            "tabela_preco.tsv",
            "template_email.eml",
            "trabalho_final.docx",
            "tutorial.docx",
            "video_aula.pdf"
    };
};


class Simulator {
public:

    static void start(unsigned int num_process, unsigned int num_printers);
};
