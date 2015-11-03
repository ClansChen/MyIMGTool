#include "IMGTableView.h"
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

IMGTableView::IMGTableView(QWidget *parent)
	:QTableView(parent)
{

}

void IMGTableView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/uri-list"))
		event->acceptProposedAction();
}

void IMGTableView::dropEvent(QDropEvent *event)
{
	QStringList paths;
	QList<QUrl> urls = event->mimeData()->urls();

	if (urls.isEmpty())
		return;

	for (auto &url : urls)
		paths.push_back(url.toLocalFile());

	emit ImportNow(paths);
}
