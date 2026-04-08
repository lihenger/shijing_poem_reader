// ChineseStyle.h
#pragma once
#include <QString>

class ChineseStyle {
public:
    static QString getMainStyle() {
        return R"(
            /* ========== 全局样式 ========== */
            QMainWindow {
                background-color: #F5F0E8;
            }
            
            QWidget {
                background-color: #F5F0E8;
                font-family: "Microsoft YaHei", "SimHei", "楷体", "KaiTi", serif;
                color: #3D2B1F;
            }
            
            /* ========== 标题样式 ========== */
            QLabel#titleLabel {
                color: #8B4513;
                font-size: 28px;
                font-weight: bold;
                font-family: "楷体", "KaiTi", serif;
                letter-spacing: 8px;
                padding: 15px;
                background: transparent;
            }
            
            QLabel {
                color: #3D2B1F;
            }
            
            /* ========== 按钮样式修复 ========== */
            QPushButton {
                background-color: #B8860B;
                color: #FFFFFF;
                border: none;
                border-radius: 20px;
                padding: 10px 20px;
                font-size: 14px;
                font-family: "楷体", "KaiTi", serif;
                min-height: 32px;
                letter-spacing: 2px;
            }
            
            QPushButton:hover {
                background-color: #CD853F;
                color: #FFFFFF;
            }
            
            QPushButton:pressed {
                background-color: #8B4513;
                color: #FFFFFF;
            }
            
            /* ========== 搜索框样式 ========== */
            QLineEdit {
                background-color: #FFF8F0;
                border: 2px solid #D2B48C;
                border-radius: 25px;
                padding: 10px 20px;
                font-size: 14px;
                color: #3D2B1F;
                selection-background-color: #B8860B;
            }
            
            QLineEdit:focus {
                border-color: #CD853F;
                background-color: #FFFFFF;
            }
            
            QLineEdit::placeholder {
                color: #A0927B;
            }
            
            /* ========== 树形控件样式修复 ========== */
            QTreeWidget {
                background-color: #FFF8F0;
                border: 1px solid #D2B48C;
                border-radius: 10px;
                outline: none;
                font-size: 13px;
                font-family: "楷体", "KaiTi", serif;
                color: #3D2B1F;
            }
            
            QTreeWidget::item {
                padding: 8px;
                border-bottom: 1px dashed #E8DCC8;
                color: #3D2B1F;
            }
            
            QTreeWidget::item:hover {
                background-color: #F5DEB3;
                color: #8B4513;
            }
            
            QTreeWidget::item:selected {
                background-color: #D2B48C;
                color: #FFFFFF;
            }
            
            QTreeWidget::branch:has-children:!has-siblings:closed,
            QTreeWidget::branch:closed:has-children:has-siblings {
                border-image: none;
            }
            
            QTreeWidget::branch:open:has-children:!has-siblings,
            QTreeWidget::branch:open:has-children:has-siblings {
                border-image: none;
            }
            
            /* ========== 列表控件样式修复 ========== */
            QListWidget {
                background-color: #FFF8F0;
                border: 1px solid #D2B48C;
                border-radius: 10px;
                outline: none;
                font-family: "楷体", "KaiTi", serif;
                font-size: 13px;
                color: #3D2B1F;
            }
            
            QListWidget::item {
                padding: 10px;
                border-bottom: 1px dashed #E8DCC8;
                color: #3D2B1F;
            }
            
            QListWidget::item:hover {
                background-color: #F5DEB3;
                color: #8B4513;
            }
            
            QListWidget::item:selected {
                background-color: #D2B48C;
                color: #FFFFFF;
            }
            
            /* ========== 文本编辑区 ========== */
            QTextEdit {
                background-color: #FFFEF5;
                border: 1px solid #D2B48C;
                border-radius: 15px;
                padding: 20px;
                font-size: 18px;
                font-family: "楷体", "KaiTi", serif;
                line-height: 2;
                color: #3D2B1F;
                selection-background-color: #F5DEB3;
            }
            
            QTextEdit:focus {
                border-color: #CD853F;
            }
            
            /* ========== 诗歌标题标签 ========== */
            QLabel#poemTitle {
                color: #8B4513;
                font-size: 32px;
                font-weight: bold;
                font-family: "楷体", "KaiTi", serif;
                letter-spacing: 6px;
                padding: 20px;
                background: transparent;
            }
            
            QLabel#sectionTitle {
                color: #A0522D;
                font-size: 16px;
                font-family: "楷体", "KaiTi", serif;
                letter-spacing: 3px;
                padding: 5px;
                background: transparent;
            }
            
            /* ========== 滚动条 ========== */
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
            
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                border: none;
                background: none;
            }
            
            QScrollBar:horizontal {
                background-color: #E8DCC8;
                height: 12px;
                border-radius: 6px;
            }
            
            QScrollBar::handle:horizontal {
                background-color: #D2B48C;
                border-radius: 6px;
                min-width: 20px;
            }
            
            QScrollBar::handle:horizontal:hover {
                background-color: #CD853F;
            }
            
            QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
                border: none;
                background: none;
            }
            
            /* ========== 分组框 ========== */
            QGroupBox {
                border: 2px solid #D2B48C;
                border-radius: 15px;
                margin-top: 15px;
                font-family: "楷体", "KaiTi", serif;
                font-size: 14px;
                font-weight: bold;
                color: #8B4513;
            }
            
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 20px;
                padding: 0 10px;
            }
            
            /* ========== QMessageBox 修复 ========== */
            QMessageBox {
                background-color: #FFF8F0;
                color: #3D2B1F;
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
            
            /* ========== 侧边栏标签 ========== */
            QLabel {
                color: #3D2B1F;
            }
        )";
    }

    static QString getSealStyle() {
        return R"(
            QLabel#sealLabel {
                color: #CD5C5C;
                font-family: "楷体", "KaiTi", serif;
                font-size: 12px;
                font-weight: bold;
                border: 1px solid #CD5C5C;
                border-radius: 2px;
                padding: 3px 8px;
                background-color: transparent;
            }
        )";
    }
};