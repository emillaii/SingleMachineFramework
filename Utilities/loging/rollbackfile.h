#ifndef ROLLBACKFILE_H
#define ROLLBACKFILE_H

#include "../utilities_global.h"
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QObject>
#include <QTextCodec>
#include <QTextStream>

/**
 * @brief The RollbackFile class
 * ���ܲ��ԣ�Releaseģʽ�£�flushImmediatelyΪtrueʱ��ÿ��logԼ1.4us��flushImmediatelyΪfalseʱ��ÿ��logԼ5.6us��
 * ��������ڼ䣬flushImmediately��Ϊtrue��ϵͳ�ȶ����Ϊfalse��
 */
class UTILITIESSHARED_EXPORT RollbackFile
{
public:
    explicit RollbackFile();
    /**
     * @brief init
     * @param folder
     * Ŀ¼������־�ļ���λ��folder/{currentDate}Ŀ¼�£����������ڼ������ʱ����־�ļ����ض�λ���µ�{currentDate}Ŀ¼��
     * @param fileName  �ļ�����������չ��
     * @param maxSize    �����ļ�����С��������С�󣬽����ļ�����������Ӻ�׺.{index}���µ�logд�뵽�½����ļ��С�
     * @param nBackupFile  ����ı����ļ����������������������ʱ����ɾ���������־�ļ�
     * @param flushImmediately  ÿ��log�Ƿ�����д�뵽�ļ�������ֻд��������������ڼ���Ϊtrue��ϵͳ�ȶ����Ϊfalse��
     */
    void init(QString folder, QString fileName, qint64 maxSize, int nBackupFile, bool flushImmediately);
    void appendLine(const QString &s);
    ~RollbackFile();

private:
    void initFileStream();
    void rollBack();
    void dateChanged();

    QDateTime dateForLogFolderName;
    QString folder;
    QString fileName;
    qint64 maxSize;
    qint64 currentSize;
    int nBackupFile;
    bool flushImmediately;
    QFile file;
    QTextStream stream;
    QMutex locker;
};

#endif    // ROLLBACKFILE_H
