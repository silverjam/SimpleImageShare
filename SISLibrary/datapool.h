#ifndef DATAPOOL_H
#define DATAPOOL_H

#include <QtCore>

class DataPool
{
public:
    DataPool();

    void poolNewData(const QBuffer&);
    void poolNewData(const QByteArray&);
    void poolNewData(const char*, qint64);

    void setChunkSize(qint64);
    inline qint64 chunkSize() { return m_chunkSize; }

    bool isDataReady() const;

    bool readPooledChunk(QByteArray&);
    bool readPooledChunk(QBuffer&);

private:
    mutable QBuffer m_buffer;
    QByteArray m_bytes;
    qint64 m_chunkSize;
};

#endif // DATAPOOL_H
