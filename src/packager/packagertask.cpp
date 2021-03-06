#include "packagertask.h"
#include <QLoggingCategory>
#include "../operations/operation.h"
#include "../operations/addoperation.h"
#include "../operations/adddirectoryoperation.h"
#include "../operations/patchoperation.h"
#include "../operations/removeoperation.h"
#include "../operations/removedirectoryoperation.h"

PackagerTask::PackagerTask(PackagerTask::Type operationType, QString path, QString oldFilename, QString newFilename)
    : QObject(), QRunnable()
{
    this->operationType = operationType;
    this->path = path;
    this->oldFilename = oldFilename;
    this->newFilename = newFilename;
    setAutoDelete(false);
}

bool PackagerTask::isRunSlow() const
{
    return operationType == Add || operationType == Patch;
}

void PackagerTask::run()
{
    try
    {
        switch (operationType) {
        case Add:
        {
            AddOperation * op = new AddOperation();
            operation = QSharedPointer<Operation>(op);
            op->create(path, newFilename, tmpDirectory);
            break;
        }
        case Patch:
        {
            PatchOperation * op = new PatchOperation();
            operation = QSharedPointer<Operation>(op);
            op->create(path, oldFilename, newFilename, tmpDirectory);
            break;
        }
        case RemoveFile:
        {
            RemoveOperation * op = new RemoveOperation();
            operation = QSharedPointer<Operation>(op);
            op->create(path, oldFilename);
            break;
        }
        case RemoveDir:
        {
            RemoveDirectoryOperation * op = new RemoveDirectoryOperation();
            operation = QSharedPointer<Operation>(op);
            op->create(path);
            break;
        }
        case AddDir:
        {
            AddDirectoryOperation * op = new AddDirectoryOperation();
            operation = QSharedPointer<Operation>(op);
            op->create(path);
            break;
        }
        }
    } catch(const QString &msg) {
        errorString = msg;
    }
    emit done();
}
