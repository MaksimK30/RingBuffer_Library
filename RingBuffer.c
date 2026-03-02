#include "RingBuffer.h"

RingBuffer* initRingBuffer(const uint32_t count, const uint32_t elementSize){
    RingBuffer *ringBuffer;

    //Проверка на чётность
    if ((count & (count - 1)) != 0)
        return NULL;

    //Выделение памяти для кольцевого буфера
    ringBuffer = (RingBuffer*)malloc(sizeof(RingBuffer));

    //Проверяем успешность выделения памяти
    if(ringBuffer == NULL){
		return NULL;	
	}

    //Выделение памяти для данных буфер
    ringBuffer->buffer = (uint8_t*)malloc(count * elementSize);

    //Проверяем успешность выделения памяти
    if(ringBuffer->buffer == NULL){
        free(ringBuffer);
        return NULL;
    }

    //Начальная инициализация полей структуры
    ringBuffer->idxRead = 0;
		ringBuffer->idxWrite = 0;
    ringBuffer->count = count;
    ringBuffer->mask = count - 1;
    ringBuffer->elementSize = elementSize;
    ringBuffer->lastError = RingBuffer_OK;

    //Возвращаем результат
	return ringBuffer;
}

RING_BUFFER_OPERATION_RESULT ringBufferIsNull(const RingBuffer *ringBuffer){

    //Если указатель на кольцевой буфер NULL
    if(ringBuffer == NULL){
        return RingBuffer_IS_NULL;
    }

    //Если указатель на данные буфера NULL
    if(ringBuffer->buffer == NULL){
        return RingBuffer_BUFFER_IS_NULL;
    }

    //Всё хорошо, если прошли проверки
    return RingBuffer_OK;
}

RING_BUFFER_OPERATION_RESULT pushRingBufferElement(RingBuffer *ringBuffer, uint8_t *value){
    //Проверяем полученый буфер
    RING_BUFFER_OPERATION_RESULT result = ringBufferIsNull(ringBuffer);
    if(result != RingBuffer_OK){
        return result;
    }

    //Проверяем указатель на помещаемое значение
    if(value == NULL){
        return RingBuffer_VALUE_IS_NULL;
    }

    //Проверка на наличие места в буфере
    if(ringBuffer->idxRead == ((ringBuffer->idxWrite + 1) & ringBuffer->mask)){
        ringBuffer->lastError = RingBuffer_BUFFER_IS_FULL;
        return RingBuffer_BUFFER_IS_FULL;
	}
	
    //Устанавливаем значение, после чего сдвигаем индекс записи на 1 элемент
    memcpy(
        &ringBuffer->buffer[ringBuffer->idxWrite * ringBuffer->elementSize],
        value,
        ringBuffer->elementSize
        );
	
	//Как только дойдём до size - обнулим счётчик
    ringBuffer->idxWrite = (ringBuffer->idxWrite + 1) & ringBuffer->mask;

    //Вернуть успех
	ringBuffer->lastError = RingBuffer_OK;
    return RingBuffer_OK;
}

RING_BUFFER_OPERATION_RESULT popRingBufferElement(RingBuffer *ringBuffer, uint8_t *value){
    //Проверяем полученый буфер
    RING_BUFFER_OPERATION_RESULT result = ringBufferIsNull(ringBuffer);
    if(result != RingBuffer_OK){
        return result;
    }

    //Проверяем указатель на возвращаемое значение
    if(value == NULL){
        return RingBuffer_VALUE_IS_NULL;
    }

    //Проверка на наличие информации для чтения
    if(ringBuffer->idxRead == ringBuffer->idxWrite){
        ringBuffer->lastError = RingBuffer_BUFFER_IS_EMPTY;
        return RingBuffer_BUFFER_IS_EMPTY;
	}
	
    //Получили из буфера значение и сдвинули индекс на 1 элемент
    memcpy(
        value,
        &ringBuffer->buffer[ringBuffer->idxRead * ringBuffer->elementSize],
        ringBuffer->elementSize
        );

	//Если дошли до size - обнуляем индекс
    ringBuffer->idxRead = (ringBuffer->idxRead + 1) & ringBuffer->mask;

    //Вернуть успех
	ringBuffer->lastError = RingBuffer_OK;
    return RingBuffer_OK;
}
