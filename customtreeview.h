#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

#include <QTreeView>

class CustomTreeView : public QTreeView
{
    Q_OBJECT

public:
    enum ItemType { ToolType, SensorType, MainMnemonicType, AdditionalMnemonicType };

    explicit CustomTreeView(QWidget *parent = nullptr);

signals:
    void addItem(int itemType);
    void editItem(int itemType, int itemId);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onAddItem();
    void onEditItem();
};

#endif // CUSTOMTREEVIEW_H
