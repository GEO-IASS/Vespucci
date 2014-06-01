/************************************************************************************
    Copyright (C) 2014 Daniel P. Foose - All Rights Reserved

    This file is part of Vespucci.

    Vespucci is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Vespucci is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Vespucci.  If not, see <http://www.gnu.org/licenses/>.
***************************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loaddataset.h"
#include "aboutdialog.h"
#include "citationdialog.h"
#include "univariatedialog.h"
#include "bandratiodialog.h"
#include "principalcomponentsdialog.h"

MainWindow::MainWindow(QWidget *parent, VespucciWorkspace *ws) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    workspace = ws;
    ui->setupUi(this);
    map_list_widget_ = this->findChild<QListWidget *>("mapsListWidget");
    dataset_list_widget_ = this->findChild<QListWidget *>("datasetsListWidget");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int response = QMessageBox::question(this,
                                         "Exit?",
                                         "Are you sure you want to exit?");

    if (response == QMessageBox::Yes) {
        event->accept();
        qApp->exit();
    }

    else {
        event->ignore();
    }
}

void MainWindow::on_actionExit_triggered()
{
    int response = QMessageBox::question(this,
                                         "Exit?",
                                         "Are you sure you want to exit?");

    if (response == QMessageBox::Yes)
        qApp->exit();
}

void MainWindow::on_actionImport_Dataset_from_File_triggered()
{
    LoadDataset *load_dataset_window = new LoadDataset(this,workspace);
    load_dataset_window->show();
}

void MainWindow::on_actionAbout_Vespucci_triggered()
{
    AboutDialog *about_window = new AboutDialog(this);
    about_window->show();
}

void MainWindow::on_actionCiting_Vespucci_triggered()
{
    CitationDialog *citation_window = new CitationDialog(this);
    citation_window->show();
}

void MainWindow::on_actionNew_Univariate_Map_triggered()
{
    int row = dataset_list_widget_->currentRow();
    UnivariateDialog *univariate_dialog = new UnivariateDialog(this, workspace, row);
    univariate_dialog->show();
}

void MainWindow::on_actionNew_Band_Ratio_Map_triggered()
{
    int row = dataset_list_widget_->currentRow();
    BandRatioDialog *band_ratio_dialog = new BandRatioDialog(this, workspace, row);
    band_ratio_dialog->show();

}

void MainWindow::on_actionPrincipal_Components_Analysis_triggered()
{
    int row = dataset_list_widget_->currentRow();
    PrincipalComponentsDialog *principal_components_dialog = new PrincipalComponentsDialog(this, workspace, row);
    principal_components_dialog->show();
}
