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
#include "univariatedialog.h"
#include "ui_univariatedialog.h"

UnivariateDialog::UnivariateDialog(QWidget *parent, VespucciWorkspace *ws, int row) :
    QDialog(parent),
    ui(new Ui::UnivariateDialog)
{
    ui->setupUi(this);
    workspace = ws;
    min_box_ = this->findChild<QLineEdit *>("minLineEdit");
    max_box_ = this->findChild<QLineEdit *>("maxLineEdit");
    name_box_ = this->findChild<QLineEdit *>("nameLineEdit");
    spectrum_plot_ = this->findChild<QCustomPlot *>("spectrumPlot");
    value_method_selector_ = this->findChild<QComboBox *>("peakComboBox");
    color_selector_ = this->findChild<QComboBox *>("gradientComboBox");
    negative_box_ = this->findChild<QCheckBox *>("negativeScoresCheckBox");
    double min = workspace->GetWavelengthMin(row);
    double max = workspace->GetWavelengthMax(row);

    min_box_->setValidator(new QDoubleValidator(min, max, 2, this));
    max_box_->setValidator(new QDoubleValidator(min, max, 2, this));
    data_ = workspace->DatasetAt(row);

    QVector<double> plot_data = data_->PointSpectrum(0);
    QVector<double> wavelength = data_->WavelengthQVector();

    spectrum_plot_->addGraph();
    spectrum_plot_->graph(0)->addData(wavelength, plot_data);


    spectrum_plot_->xAxis->setRange(data_->WavelengthRange());
    spectrum_plot_->yAxis->setRange(data_->PointSpectrumRange(0));

}

UnivariateDialog::~UnivariateDialog()
{
    delete ui;
}

void UnivariateDialog::on_buttonBox_accepted()
{
    double entered_min = min_box_->text().toDouble();
    double entered_max = min_box_->text().toDouble();

    cout << "call to SpecMap::FindRange" << endl;

    vector<int> range = data_.data()->FindRange(entered_min, entered_max);

    int min = range[0];
    int max = range[1];

    QString name = name_box_->text();
    QString value_method = value_method_selector_->currentText();

    int color_index = color_selector_->currentIndex();
    QMessageBox::information(0, "Debug Notification", "Call to SpecMap::Univariate()");
    data_->Univariate(min, max, name, value_method, color_index);

}
