#pragma once
#include <QString>

class ChineseStyle {
public:
    static QString getMainStyle() {
        return R"(
            QMainWindow {
                background-color: #F5F0E8;
            }
            
            QWidget {
                background-color: #F5F0E8;
                font-family: "Microsoft YaHei", "SimHei", "楷体", "KaiTi", serif;
                color: #3D2B1F;
            }
            
            QLabel {
                color: #3D2B1F;
            }
            
            QPushButton {
                background-color: #B8860B;
                color: #FFFFFF;
                border: none;
                border-radius: 20px;
                padding: 10px 20px;
                font-size: 14px;
                font-family: "楷体", "KaiTi", serif;
                min-height: 32px;
            }
            
            QPushButton:hover {
                background-color: #CD853F;
                color: #FFFFFF;
            }
            
            QPushButton:pressed {
                background-color: #8B4513;
                color: #FFFFFF;
            }
            
            QLineEdit {
                background-color: #FFF8F0;
                border: 2px solid #D2B48C;
                border-radius: 25px;
                padding: 10px 20px;
                font-size: 14px;
                color: #3D2B1F;
            }
            
            QLineEdit:focus {
                border-color: #CD853F;
                background-color: #FFFFFF;
            }
            
            QLineEdit::placeholder {
                color: #A0927B;
            }
            
            QTreeWidget {
                background-color: #FFF8F0;
                border: 1px solid #D2B48C;
                border-radius: 10px;
                color: #3D2B1F;
                font-family: "楷体", "KaiTi", serif;
            }
            
            QTreeWidget::item {
                padding: 8px;
                border-bottom: 1px dashed #E8DCC8;
            }
            
            QTreeWidget::item:hover {
                background-color: #F5DEB3;
                color: #8B4513;
            }
            
            QTreeWidget::item:selected {
                background-color: #D2B48C;
                color: #FFFFFF;
            }
            
            QListWidget {
                background-color: #FFF8F0;
                border: 1px solid #D2B48C;
                border-radius: 10px;
                color: #3D2B1F;
                font-family: "楷体", "KaiTi", serif;
            }
            
            QListWidget::item {
                padding: 10px;
                border-bottom: 1px dashed #E8DCC8;
            }
            
            QListWidget::item:hover {
                background-color: #F5DEB3;
                color: #8B4513;
            }
            
            QListWidget::item:selected {
                background-color: #D2B48C;
                color: #FFFFFF;
            }
            
            QTextEdit {
                background-color: #FFFEF5;
                border: 1px solid #D2B48C;
                border-radius: 15px;
                padding: 20px;
                font-size: 18px;
                font-family: "楷体", "KaiTi", serif;
                line-height: 2;
                color: #3D2B1F;
            }
            
            QScrollBar:vertical {
                background-color: #E8DCC8;
                width: 12px;
                border-radius: 6px;
            }
            
            QScrollBar::handle:vertical {
                background-color: #D2B48C;
                border-radius: 6px;
                min-height: 20px;
            }
            
            QScrollBar::handle:vertical:hover {
                background-color: #CD853F;
            }
            
            QMessageBox {
                background-color: #FFF8F0;
            }
            
            QMessageBox QLabel {
                color: #3D2B1F;
            }
            
            QMessageBox QPushButton {
                background-color: #B8860B;
                color: #FFFFFF;
                min-width: 80px;
            }
            
            QMessageBox QPushButton:hover {
                background-color: #CD853F;
            }
        )";
    }
};