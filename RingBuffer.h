#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*!
 * @brief Перечисление состояний буфера
 * @details Перечисление с типами результатов
 * проведения операций над буфером
 */
typedef enum {
    RingBuffer_OK, ///< Операция выполнена успешно
    RingBuffer_IS_NULL, ///< Указатель на кольцевой буфер равен NULL
    RingBuffer_BUFFER_IS_NULL, ///< Указатель на данные равен NULL
    RingBuffer_BUFFER_IS_EMPTY, ///< Нет данных в буфере
    RingBuffer_BUFFER_IS_FULL, ///< Буфер переполнен
    RingBuffer_VALUE_IS_NULL ///< Значение пусто
} RING_BUFFER_OPERATION_RESULT;

/*!
 * @brief Структура кольцевой буфер
 * @details Структура описывает поля кольцевого
 * буфера данных
 */
typedef struct {
    uint8_t *buffer; ///< Ссылка на данные буфера
    volatile uint32_t idxRead; ///< Индекс чтения данных
    volatile uint32_t idxWrite; ///< Индекс записи данных
    uint32_t count; ///< Количество элементов. Обязательно должно быть равно степени 2, иначе буфер сломается
    uint32_t mask; ///< Маска количества элементов. Необходима для автоматического перехода к началу буфера
    uint32_t elementSize; ///< Размер экземпляра хранимого типа данных
    RING_BUFFER_OPERATION_RESULT lastError; ///< Результат последнего выполнения операции над буфером
	
} RingBuffer;

/*!
 * @brief Инициализировать буфер
 * @details Функция, преднастраивающая и
 * инициализирующая буфер начальными значениями
 * @param count Количество элементов, хранимых в буфере. Должно быть кратно степени 2
 * @param elementSize Размер экземпляра типа данных хранимых элементов
 * @return Возвращает указатель на созданный объект в памяти
 */
RingBuffer* initRingBuffer(const uint32_t count, const uint32_t elementSize);

/*!
 * @brief Буфер инициализирован
 * @details Проверка инициализации переданного буфера
 * @param ringBuffer Указатель на исследуемый буфер
 * @return Возвращает статус проведения операции над буфером
 */
RING_BUFFER_OPERATION_RESULT ringBufferIsNull(const RingBuffer *ringBuffer);

/*!
 * @brief Вставить элемент в буфер
 * @details Вставляет переданный элемент в буфер.
 * Проводит проверку на пустоту переданного буфера, свободного места
 * @param ringBuffer Указатель на буфер, с которым работаем
 * @param value Указатель на начало объекта, помещаемого в буфер
 * @return Возращает результат проведения операции над буфером
 */
RING_BUFFER_OPERATION_RESULT pushRingBufferElement(RingBuffer *ringBuffer, uint8_t *value);

/*!
 * @brief Извлечь объект из буфера
 * @details Функция проверяет буфер и извлекает из него следующий объект
 * @param ringBuffer Указатель на буфер, с которым работаем
 * @param value Указатель на объект, в который будет
 * записано полученое значение
 * @return Возращает результат проведения операции над буфером
 */
RING_BUFFER_OPERATION_RESULT popRingBufferElement(RingBuffer *ringBuffer, uint8_t *value);

#endif
