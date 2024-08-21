#include "customtreeview.h"
#include <QContextMenuEvent>
#include <QMenu>

CustomTreeView::CustomTreeView(QWidget *parent)
    : QTreeView(parent)
{}

void CustomTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *addAction = menu.addAction("Add");
    QAction *editAction = menu.addAction("Edit");

    connect(addAction, &QAction::triggered, this, &CustomTreeView::onAddItem);
    connect(editAction, &QAction::triggered, this, &CustomTreeView::onEditItem);

    menu.exec(event->globalPos());
}

void CustomTreeView::onAddItem()
{
    QModelIndex index = currentIndex();
    if (!index.isValid()) {
        emit addItem(ToolType); // Default to ToolType
        return;
    }

    // Determine item type based on index
    // Example logic, adapt based on your model structure
    int itemType = ToolType; // Default type
    if (index.data().toString().contains("Sensor")) {
        itemType = SensorType;
    } else if (index.data().toString().contains("MainMnemonic")) {
        itemType = MainMnemonicType;
    } else if (index.data().toString().contains("AdditionalMnemonic")) {
        itemType = AdditionalMnemonicType;
    }

    emit addItem(itemType);
}

void CustomTreeView::onEditItem()
{
    QModelIndex index = currentIndex();
    if (!index.isValid())
        return;

    int itemType = ToolType;
    int itemId = index.data(Qt::UserRole + 1).toInt(); // Assuming ID is stored here

    if (index.data().toString().contains("Sensor")) {
        itemType = SensorType;
    } else if (index.data().toString().contains("MainMnemonic")) {
        itemType = MainMnemonicType;
    } else if (index.data().toString().contains("AdditionalMnemonic")) {
        itemType = AdditionalMnemonicType;
    }

    emit editItem(itemType, itemId);
}
