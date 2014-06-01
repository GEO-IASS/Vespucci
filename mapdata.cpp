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

#include "mapdata.h"


MapData::MapData(QString x_axis_description,
                 QString y_axis_description,
                 colvec x,
                 colvec y,
                 colvec results,
                 SpecMap *parent,
                 QString *directory,
                 QCPColorGradient gradient,
                 int source_index)
{
    name_ = parent->name();
    directory_ = directory;
    map_display_ = new MapViewer(name_, directory_, this);
    map_qcp_ = map_display_->findChild<QCustomPlot *>("mapView");
    map_ = new QCPColorMap(map_qcp_->xAxis, map_qcp_->yAxis);
    map_->setGradient(gradient);
    map_qcp_->addPlottable(map_);
    map_->data()->setKeyRange(parent->KeyRange());
    map_->data()->setValueRange(parent->ValueRange());
    map_->data()->setKeySize(parent->KeySize());
    map_->data()->setValueSize(parent->ValueSize());

    for(unsigned int i = 0; i<x.n_elem; ++i){
        map_->data()->setData(x(i), y(i), results(i));
    }
    map_->rescaleDataRange();
    map_qcp_->rescaleAxes();
    this->CreateImage(gradient, false);

    spectrum_display_ = new SpectrumViewer;
    spectrum_qcp_ = spectrum_display_->findChild<QCustomPlot *>("spectrum");
    //create the objects the pointers point to
    x_axis_description_ = x_axis_description;
    y_axis_description_ = y_axis_description;
    dataset_ = parent;
    source_index_ = source_index;
}

///
/// \brief MapData::~MapData
///Deletes everything the new keyword is used on in this object.
/// Destructor triggered when this is removed from SpecMap list.
MapData::~MapData()
{
    delete map_display_;
    delete map_;
    delete spectrum_display_;
    delete new_color_scale_;
}

QString MapData::name()
{
    return name_;
}

QString MapData::type()
{
    return type_;
}

int MapData::source_index()
{
    return source_index_;
}

void MapData::set_type(QString type)
{
    type_ = type;
}

void MapData::set_name(QString name, QString type)
{
    name_ = name;
    type_ = type;
    QString window_title = dataset_->name() + "—" + name + " " + type;
    map_display_->setWindowTitle(window_title);
}

void MapData::ShowMapWindow()
{
    if(!map_display_->isVisible()){
        map_display_->show();
    }
    if(!map_display_->isActiveWindow()){
        map_display_->activateWindow();
    }
}

//Plots the data on the MapViewer window
void MapData::SetMapData(QCPColorMapData *map_data)
{
    map_->setData(map_data);
}

void MapData::CreateImage(QCPColorGradient color_scheme, bool interpolation)
{

    map_->setGradient(color_scheme);
    map_->rescaleDataRange(true);
    map_qcp_->rescaleAxes(true);

    new_color_scale_ = new QCPColorScale(map_qcp_);
    new_color_scale_->setGradient(color_scheme);
    new_color_scale_->setDataRange(map_->dataRange());
    map_qcp_->plotLayout()->addElement(0, 1, new_color_scale_);

    color_scale_ = map_qcp_->plotLayout()->element(0, 1);

    map_->setInterpolate(interpolation);
    int key_size = map_->data()->keySize();
    int value_size = map_->data()->valueSize();
    key_size *= 9;
    value_size *= 9;
    map_qcp_->resize(key_size, value_size);
    key_size +=50;
    value_size +=50;
    map_display_->resize(key_size, value_size);
}

void MapData::SetXDescription(QString description)
{
    x_axis_description_ = description;
}

void MapData::SetYDescription(QString description)
{
    y_axis_description_ = description;
}

bool MapData::interpolate()
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    return map_->interpolate();
}

void MapData::setInterpolate(bool enabled)
{
    if (!map_qcp_->hasPlottable(map_)){
        map_qcp_->addPlottable(map_);
    }
    map_->setInterpolate(enabled);
    map_qcp_->replot();
}

void MapData::ShowColorScale(bool enabled)
{
    if (enabled){
        if (!map_qcp_->plotLayout()->element(0,1)->visible()){
            map_qcp_->plotLayout()->element(0, 1)->setVisible(true);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }

    else{
        if (map_qcp_->plotLayout()->element(0, 1)->visible()){
            map_qcp_->plotLayout()->element(0, 1)->setVisible(false);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }
}

void MapData::ShowAxes(bool enabled)
{
    if (enabled){
        if (!map_qcp_->xAxis->visible()){
            map_qcp_->xAxis->setVisible(true);
            map_qcp_->yAxis->setVisible(true);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }

    else{
        if (map_qcp_->xAxis->visible()){
            map_qcp_->xAxis->setVisible(false);
            map_qcp_->yAxis->setVisible(false);
            map_qcp_->replot();
            map_qcp_->repaint();
        }
    }
}

bool MapData::saveBmp(const QString &fileName, int width, int height, double scale)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveBmp(fileName, width, height, scale);

    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

bool MapData::savePng(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    bool success = map_qcp_->savePng(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

bool MapData::saveJpg(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::white);
    map_qcp_->replot();
    bool success = map_qcp_->saveJpg(fileName, width, height, scale, quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

bool MapData::saveTiff(const QString &fileName, int width, int height, double scale, int quality)
{
    map_qcp_->setBackground(Qt::transparent);
    map_qcp_->replot();
    bool success = map_qcp_->saveRastered(fileName, width, height, scale, "TIF", quality);
    map_qcp_->setBackground(map_display_->palette().window());
    map_qcp_->replot();
    return success;
}

///
/// \brief MapData::RemoveThis
///Triggers the SpecMap object to remove this from the list.  Since SpecMap
/// stores MapData objects as shared pointers, and only one object (the map list)
/// contains this pointer, this removal results in this object being deleted.
void MapData::RemoveThis()
{
    dataset_->RemoveMapAt(source_index_);
}
