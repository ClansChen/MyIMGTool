#pragma once
#include <QTableView>

class MyTableView :public QTableView
{
public:
	MyTableView(QWidget *parent = nullptr);

protected:
/*
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);*/
};
