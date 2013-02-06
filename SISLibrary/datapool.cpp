#include "datapool.h"

#include <cstdint>

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DataPool::DataPool()
    : m_chunkSize(INT64_MAX)
{
    m_buffer.setBuffer(&m_bytes);
    m_buffer.open(QIODevice::ReadWrite);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
DataPool::poolNewData(const QBuffer & buf)
{
    m_buffer.write(buf.data());
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
DataPool::poolNewData(const QByteArray& bytes)
{
    m_buffer.write(bytes);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
DataPool::poolNewData(const char* data, qint64 len)
{
    m_buffer.write(data, len);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
DataPool::isDataReady() const
{
    qint64 pos = m_buffer.pos();

    m_buffer.seek(0);
    qint64 available = m_buffer.bytesAvailable();

    m_buffer.seek(pos);

    return available >= m_chunkSize;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
DataPool::readPooledChunk(QByteArray& bytes)
{
    QBuffer buffer(&bytes, 0);
    buffer.open(QIODevice::ReadWrite);

    return readPooledChunk(buffer);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool
DataPool::readPooledChunk(QBuffer& buffer)
{
    Q_ASSERT( m_buffer.isOpen() && m_buffer.isWritable() );

    if ( ! buffer.isOpen() || ! buffer.isWritable() )
        return false;

    int pos = m_buffer.pos();
    m_buffer.seek(0);

    if ( m_buffer.bytesAvailable() < m_chunkSize )
    {
        m_buffer.seek(pos);
        return false;
    }

    bool bWrote = buffer.write(m_bytes.data(), m_chunkSize) == m_chunkSize;
    if ( ! bWrote )
        return false;

    m_buffer.close();
    m_bytes.remove(0, m_chunkSize);

    m_buffer.open(QIODevice::ReadWrite);

    return true;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
DataPool::setChunkSize(qint64 chunkSize)
{
    m_chunkSize = chunkSize;
}
