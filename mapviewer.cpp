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

#include "mapviewer.h"
#include "ui_mapviewer.h"


MapViewer::MapViewer(QString name, QString *directory, MapData *parent):
    QMainWindow(0),
    ui(new Ui::MapViewer)
{
    ui->setupUi(this);
    name_ = name;
    directory_ = directory;

    qcp_= this->findChild<QCustomPlot *>("mapView");
    qcp_->setBackground(this->palette().window());
    parent_ = parent;
    //color_map_ = qobject_cast<QCPColorMap *>(color_map_abs);
}

MapViewer::~MapViewer()
{
    delete ui;
}


void MapViewer::closeEvent(QCloseEvent * event)
{
    // the call to SpecMap::RemoveMapAt() results in program crashing
 /*
    int response = QMessageBox::question(this,
                                         "Close Map?",
                                         "Are you sure you want to close this"
                                         " map?  Map will be deleted.");
    if (response == QMessageBox::Yes){
        event->accept();
        parent_->RemoveThis();
    }
    else{
        event->ignore();
    }
    */
    event->ignore();
    this->showMinimized();
}

void MapViewer::on_actionInterpolate_triggered()
{
    //see on_actionInterpolate_toggled
}

void MapViewer::on_actionInterpolate_toggled(bool arg1)
{
    parent_->setInterpolate(arg1);
}


void MapViewer::on_actionSave_Image_As_triggered()
{
    QString path = *directory_;
    path = path + "/" + name_;

    //this almost looks as bad as putting it all on one line.
    QString filename =
            QFileDialog::getSaveFileName(this,
                                         tr("Save File"),
                                         path,
                                         tr("Tagged Image File Format (*.tif);; "
                                            "Windows Bitmap (*.bmp);; "
                                            "Portable Network Graphics (*.png);; "
                                            "JPEG (*.jpg)"));

    QStringList filename_list = filename.split(".");
    QString extension = filename_list.last();

    //this method of determining type may not be valid on non-Win platforms
    //check this on GNU/Linux and Mac OSX later.

    if (extension == "bmp")
        parent_->saveBmp(filename, 0, 0, 1.0);

    else if (extension == "png"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_->savePng(filename, 0, 0, 1.0, quality);
    }

    else if (extension == "jpg"){
        bool ok;
        int quality = QInputDialog::getInt(this, "Enter Quality",
                                           "Quality (%)",
                                           80, 0, 100, 1, &ok);
        if (ok)
            parent_->saveJpg(filename, 0, 0, 1.0, quality);
    }

    else{
        //default to tif, force extension (for Windows compatability)
        if (extension != "tif")
            filename.append(".tif");
        bool ok;
        int quality = QInputDialog::getInt(this,
                                           "Compression",
                                           "Enter 0 for no compression,"
                                           "1 for LZW lossless compression",
                                           0, 0, 1, 1, &ok);
        if (ok)
            parent_->saveTiff(filename, 0, 0, 1.0, quality);
    }

}

void MapViewer::on_actionShow_Axes_toggled(bool arg1)
{
    parent_->ShowAxes(arg1);
}

void MapViewer::on_actionShow_Color_Scale_toggled(bool arg1)
{
    parent_->ShowColorScale(arg1);
}
