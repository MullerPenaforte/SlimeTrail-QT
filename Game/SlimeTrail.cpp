#include "SlimeTrail.h"
#include "ui_SlimeTrail.h"

#include <iostream>

#include <QDebug>
#include <QMessageBox>
#include <QActionGroup>
#include <QSignalMapper>
#include <math.h>

int linha =3; //marcador para linha
int coluna=4; //marcador para coluna
int count =1; //contador para controle de jogadas possiveis
int ab =1; // "bool" para executar uma função apenas 1 vez


SlimeTrail::Player otherPlayer(SlimeTrail::Player player) {
    return (player == SlimeTrail::RedPlayer ?
                    SlimeTrail::BluePlayer : SlimeTrail::RedPlayer);
}

SlimeTrail::SlimeTrail(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SlimeTrail),
      m_player(SlimeTrail::RedPlayer) {

    ui->setupUi(this);

    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(reset()));
    QObject::connect(ui->actionQuit, SIGNAL(triggered(bool)), qApp, SLOT(quit()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAbout()));

    QSignalMapper* map = new QSignalMapper(this);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QString holeName = QString("hole%1%2").arg(row).arg(col);
            Hole* hole = this->findChild<Hole*>(holeName);
            Q_ASSERT(hole != nullptr);
            Q_ASSERT(hole->row() == row && hole->col() == col);

            m_board[row][col] = hole;


            int id = row * 8 + col;
            map->setMapping(hole, id);

            QObject::connect(hole, SIGNAL(clicked(bool)), map, SLOT(map()));

        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    //m_board[3][4]->setEnabled(false);
    QObject::connect(map, SIGNAL(mapped(int)), this, SLOT(play(int)));
#else
    QObject::connect(map, SIGNAL(mappedInt(int)), this, SLOT(play(int)));
#endif

    // When the turn ends, switch the player.
   QObject::connect(this, SIGNAL(turnEnded()), this, SLOT(switchPlayer()));

    this->reset();

    this->adjustSize();
    this->setFixedSize(this->size());
}

SlimeTrail::~SlimeTrail() {
    delete ui;
}

void SlimeTrail::play(int id) {

    Hole* hole = m_board[id / 8][id % 8];


    qDebug() << "clicked on: " << hole->objectName();
    for (int row = 0; row < 8; ++row) { //desabilita "os movimentos"...
        for (int col = 0; col < 8; ++col) {//... que não são permitidos

            m_board[row][col]->setMarked(false);
            m_board[row][col]->setDisabled(true);
        }
    }


    hole->setState(Hole::WhiteState); //muda a cor da bolinha posicao atual
    hole->setEnabled(false);

    m_board[linha][coluna]->setState(Hole::BlackState);//muda a cor da bolinha posicao anterior
    linha = id / 8;
    coluna = id % 8;    
    if(m_board[linha][coluna]->WhiteState){
       // linha = *superId / 8;
       // coluna = *superId %8;

        if((coluna ==7 && linha == 0)){ //condicao de vitoria

            QMessageBox::information(this, "Vencedor", "O jogador azul venceu!");

            QApplication::quit();


        }else if((coluna == 0 && linha == 7)){ //condicao de vitoria 2
            QMessageBox::information(this, "Vencedor", "O jogador vermelho venceu!");

            QApplication::quit();

        }

       else if(coluna ==0 && linha !=0){ //limitar os locais onde pode mover

            m_board[linha-1][coluna]->setMarked(true);
            m_board[linha-1][coluna+1]->setMarked(true);
            m_board[linha][coluna+1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);
            m_board[linha+1][coluna+1]->setMarked(true);

            m_board[linha-1][coluna]->setDisabled(false);
            m_board[linha-1][coluna+1]->setDisabled(false);
            m_board[linha][coluna+1]->setDisabled(false);
            m_board[linha+1][coluna]->setDisabled(false);
            m_board[linha+1][coluna+1]->setDisabled(false);


            }else if(linha ==0 && coluna !=0)
            {
            m_board[linha][coluna-1]->setMarked(true);
            m_board[linha+1][coluna-1]->setMarked(true);
            m_board[linha][coluna+1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);
            m_board[linha+1][coluna+1]->setMarked(true);

            m_board[linha][coluna-1]->setDisabled(false);
            m_board[linha+1][coluna-1]->setDisabled(false);
            m_board[linha][coluna+1]->setDisabled(false);
            m_board[linha+1][coluna]->setDisabled(false);
            m_board[linha+1][coluna+1]->setDisabled(false);
            }else if(coluna == 7 && linha !=7){
            m_board[linha-1][coluna-1]->setMarked(true);
            m_board[linha-1][coluna]->setMarked(true);
            m_board[linha][coluna-1]->setMarked(true);
            m_board[linha+1][coluna-1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);

            m_board[linha-1][coluna-1]->setDisabled(false);
            m_board[linha-1][coluna]->setDisabled(false);
            m_board[linha][coluna-1]->setDisabled(false);
            m_board[linha+1][coluna-1]->setDisabled(false);
            m_board[linha+1][coluna]->setDisabled(false);

            }else if(linha == 7 && coluna !=7){
            m_board[linha-1][coluna-1]->setMarked(true);
            m_board[linha-1][coluna]->setMarked(true);
            m_board[linha-1][coluna+1]->setMarked(true);
            m_board[linha][coluna-1]->setMarked(true);
            m_board[linha][coluna+1]->setMarked(true);

            m_board[linha-1][coluna-1]->setDisabled(false);
            m_board[linha-1][coluna]->setDisabled(false);
            m_board[linha-1][coluna+1]->setDisabled(false);
            m_board[linha][coluna-1]->setDisabled(false);
            m_board[linha][coluna+1]->setDisabled(false);

            }else if (coluna ==0 && linha ==0){


            m_board[linha][coluna+1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);
            m_board[linha+1][coluna+1]->setMarked(true);

            m_board[linha][coluna+1]->setDisabled(false);
            m_board[linha+1][coluna]->setDisabled(false);
            m_board[linha+1][coluna+1]->setDisabled(false);

            } else if (coluna ==7 && linha ==7){
            m_board[linha-1][coluna-1]->setMarked(true);
            m_board[linha-1][coluna]->setMarked(true);
            m_board[linha][coluna-1]->setMarked(true);

            m_board[linha-1][coluna-1]->setDisabled(false);
            m_board[linha-1][coluna]->setDisabled(false);
            m_board[linha][coluna-1]->setDisabled(false);
            }
            else{
            m_board[linha-1][coluna-1]->setMarked(true);
            m_board[linha-1][coluna]->setMarked(true);
            m_board[linha-1][coluna+1]->setMarked(true);
            m_board[linha][coluna-1]->setMarked(true);
            m_board[linha+1][coluna-1]->setMarked(true);
            m_board[linha][coluna+1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);
            m_board[linha+1][coluna+1]->setMarked(true);

            m_board[linha-1][coluna-1]->setDisabled(false);
            m_board[linha-1][coluna]->setDisabled(false);
            m_board[linha-1][coluna+1]->setDisabled(false);
            m_board[linha][coluna-1]->setDisabled(false);
            m_board[linha+1][coluna-1]->setDisabled(false);
            m_board[linha][coluna+1]->setDisabled(false);
            m_board[linha+1][coluna]->setDisabled(false);
            m_board[linha+1][coluna+1]->setDisabled(false);
         }
    }
    int x=0,y=0;

        for (x = 0; x< 8; ++x) {
            for (y = 0; y < 8; ++y) {
                if(m_board[x][y]->state() == m_board[x][y]->BlackState){
                   m_board[x][y]->setDisabled(true); //impedir movimento onde tem bolas pretas

                }
                if(m_board[x][y]->isMarked() && m_board[x][y]->isEnabled()){
                    count++; //marcar falta de movimentos para condição empate
                }
             }

                }

                if(count==0){
            QMessageBox::information(this, "Empatado", "Jogo empatado!");
            QApplication::quit();
            }


        count=0;

            // FIXME: Only neighboor holes should be marked.


    emit turnEnded();

}

void SlimeTrail::switchPlayer() {
    // Switch the player.
    m_player = otherPlayer(m_player);

    // Finally, update the status bar.
    this->updateStatusBar();
}

void SlimeTrail::reset() {
    // Reset board.
    linha = 3;
    coluna =4;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Hole* hole = m_board[row][col];
            hole->reset();
            m_board[row][col]->setMarked(false);
            hole->setEnabled(true);

            // FIXME: Only neighboor holes should be marked.

            if(m_board[linha][coluna]->WhiteState){
               // linha = *superId / 8;
               // coluna = *superId %8;


                m_board[linha-1][coluna-1]->setMarked(true);
                m_board[linha-1][coluna]->setMarked(true);
                m_board[linha-1][coluna+1]->setMarked(true);
                m_board[linha][coluna-1]->setMarked(true);
                m_board[linha+1][coluna-1]->setMarked(true);

            m_board[linha][coluna+1]->setMarked(true);
            m_board[linha+1][coluna]->setMarked(true);
            m_board[linha+1][coluna+1]->setMarked(true);
            m_board[linha][coluna]->setMarked(false);
            }
        }
    }

    // Mark the starting position.
    m_board[linha][coluna]->setState(Hole::WhiteState);
    m_board[3][4]->setEnabled(false);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            m_board[row][col]->setMarked(false);
            m_board[row][col]->setDisabled(true);
        }
    }

    if(ab==1){  //habilitar os primeiros movimentos
    m_board[3-1][4-1]->setDisabled(false);
    m_board[3-1][4]->setDisabled(false);
    m_board[3-1][4+1]->setDisabled(false);
    m_board[3][4-1]->setDisabled(false);
    m_board[3+1][4-1]->setDisabled(false);
    m_board[3][4+1]->setDisabled(false);
    m_board[3+1][4]->setDisabled(false);
    m_board[3+1][4+1]->setDisabled(false);

    m_board[3-1][4-1]->setMarked(true);
    m_board[3-1][4]->setMarked(true);
    m_board[3-1][4+1]->setMarked(true);
    m_board[3][4-1]->setMarked(true);
    m_board[3+1][4-1]->setMarked(true);
    m_board[3][4+1]->setMarked(true);
    m_board[3+1][4]->setMarked(true);
    m_board[3+1][4+1]->setMarked(true);
    ab++;
    }

    // Reset the player.
    m_player = SlimeTrail::RedPlayer;

    // Finally, update the status bar.
    this->updateStatusBar();
}

void SlimeTrail::showAbout() {
    QMessageBox::information(this, tr("Sobre"), tr("Rastros\n\nAlline e Müller - andrei@cefetmg.br"));
}

void SlimeTrail::updateStatusBar() {
    QString player(m_player == SlimeTrail::RedPlayer ? "Vermelho" : "Azul");
    ui->statusbar->showMessage(tr("Vez do Jogador %2").arg(player));
}
