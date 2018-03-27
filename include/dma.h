#define DMA_BUSY  1
#define DMA_ERROR 2
#define DMA_READY 0 
#define DMA_OK    0 

typedef int(*DmaCallback2)();

#ifdef __cplusplus
		extern "C" {
#endif

	typedef int(*DmaCallback)();
	/**
	 *  \brief Brief
	 *  
	 *  \param [in] dstptr Parameter_Description
	 *  \param [in] srcptr Parameter_Description
	 *  \param [in] n Parameter_Description
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmacinit( int* dstptr,  int* srcptr, int n ); // в адресах ARM
	
	/**
	 *  \brief Brief
	 *  
	 *  \return Return_Description
	 *  
	 *  \details Details
	 */
	int xdmac0(); // возвращает текущий счётчик (0 если передача окончена )
	
	
	int dmaCheck(void* src, void* dst, int size32);
	int dmaInit (void* src, void* dst, int size32, DmaCallback func);
	//void dmaInitBias(int n, unsigned src, unsigned dstq, int row2read, int bias2read, int row2wrt, int bias2wrt);
	void dmaMatrixInit(void* src, int width, int height, int srcStride,  void* dst, int dstStride );
	int  dmaStatus();

	//---------------------------------
	//int  halInitSingleDMA(void*  src,  void*  dst,  int  size32,  DmaCallback* func, int channel=0);
	//int  halInitPacketDMA(void** src,  void** dst,  int* size32,  DmaCallback* func, int channel=0);
	//int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1, DmaCallback func, int channel=0);
	//int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32, DmaCallback func, int channel=0);
	//int  halStatusDMA();
	
	/**
	 *  \brief функция инициализирует модуль ПДП
	 *  \param [in] адрес источника
	 *  \param [in] адрес приемника
	 *  \param [in] колличество 32 битных слов для перессылки
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет 
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший наименший четный) 
	 */

	int  halInitSingleDMA(void*  src,  void*  dst,  int  size32);
	/**
	 *  \brief функция инициализирует модуль ПДП для записи двух массивов
	 *  \param [in] адрес первого источника
	 *  \param [in] адрес второго источника
	 *  \param [in] адрес первого приемника
	 *  \param [in] адрес второго приемника
	 *  \param [in] колличество 32 битных слов для перессылки первой пары массивов
	 *  \param [in] колличество 32 битных слов для перессылки второй пары массивов
	 *	\restr 
	 *	src0 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst0 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>src1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst1 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>intSize0 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший меньший четный) 
	 *	<p>intSize1 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший меньший четный) 
	 */
	int  halInitDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	/**
	 *  \brief функция инициализирует модуль ПДП для записи пакета массивов
	 *  \param [in] адрес массива, содержащего адреса источников
	 *  \param [in] адрес массива, содержащего адреса приемников
	 *  \param [in] колличество 32 битных слов для перессылки первой пары массивов
	 *  \param [in] колличество 32 битных слов для перессылки второй пары массивов
	 *	\restr 
	 *  src0 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst0 адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>src1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>dst1 адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>size32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 */
	int  halInitPacketDMA(void** src,  void** dst,  int* size32);
	/**
	 *  \brief функция инициализирует модуль ПДП для копирования матрицы
	 *  \param [in] адрес массива, содержащего адреса источников
	 *  \param [in] длинна строки матрицы в 32 битных словах (ширина матрицы в 32 битных словах)
	 *  \param [in] колличество строк матрицы (высота матрицы)
	 *  \param [in] растояние между соседними сторками матрицы в массиве источника (src stride в 32 биьных словах)
	 *  \param [in] адрес массива приемника
	 *  \param [in] растояние между соседними сторками матрицы в массиве приемника (dst stride в 32 биьных словах)
	 *	\restr 
	 *  src адрес должен быть четным в противном случае ПДП зависнет
	 *	<p>width должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 *	<p>srcStride32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 *	<p>dst адрес должен быть четным в противном случае ПДП зависнет  
	 *	<p>dstStride32 должен быть четным в противном случае ПДП скопирует на 1 меньше (ближайший четный) 
	 */
	int  halInitMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	/**
		* \brief Функция устанавливае функцию, которая вызовется после отработки ПДП (callBack).
		*	<p> для выключения callback можно передать в функцию 0, тогда callback не будет вызыватся.
		* <p>Для работы callback необходимо разрешить внешние прерывания для процессора вызвав функцию
		* halEnbExtInt() и на плате 12101 разрешить обработку прерываний от ПДП в обработчике прерываний
		* вызвав функцию halMaskIntContMdma_mc12101().
		*
	*/
	void halSetCallbackDMA(DmaCallback user_callback);
	/**
		* \brief Функция выполняет инициализацию программы обработки перывания и должна быть вызвана один раз перед запуском ПДП 
		* в случа необходимости использования callBack. 
		*	<p>ВАЖНО! Данная функция анализирует регистр pswr и в случае если он запрещает внешние прерывания то фунция не инициализирует программу а возвращает 10 
		* \return
		* 0  функция инициализировала программу в обработчике прерываний 
		* <p>10 функция не инициализировала программу по причине запрещенных внешних прерываний 
	*/
	int  halInitDMA();
	/**
		* \brief Функция разрешает обработку внешних прерывания у процессорного ядра на котором она (функция) была вызвана
	*/

	void halEnbExtInt();
	/**
		* \brief Функция запрещает обработку внешних прерывания у процессорного ядра на котором она (функция) была вызвана
	*/
	void halDisExtInt();
	/**
		* \brief Функция возвращает 0 в случае если ПДП закончил работу
		* для функции halInitSingleDMA() возвращает колличество оставшихся для копирования элементов
	*/
	int  halStatusDMA();
	/**
		* \brief Функция разрешает обработку прерывания от ПДП (MDMA) в обработчике прерывания на плате 12101 
	*/
	void halMaskIntContMdma_mc12101();
	// test of params
	// error code description
	////////single DMA
	// 1 stands for src is odd 
	// 2 stands for dst is odd
	// 3 stands for size is odd
	
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitSingleDMA() 
		* \return 
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>3 - нечетное колличество 32 битных слов для копирования
	*/
	int halTestParamSingleDMA(void* src, void* dst, int size);
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitDoubleDMA() 
		* \return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - нечетный адрес массива второго источника
		* <p>3 - нечетный адрес массива первого приемника
		* <p>4 - нечетный адрес массива второго приемника
		* <p>5 - нечетное колличество 32 битных слов для копирования первой пары массивов
		* <p>6 - нечетное колличество 32 битных слов для копирования первой второй массивов
	*/

	// error code description
	///// double dma
	// 1 stands for src0 is odd
	// 2 stands for src1 is odd
	// 3 stands for dst0 is odd
	// 4 stands for dst1 is odd
	// 5 stands for intSize0 is odd
	// 6 stands for intSize1 is odd
	int halTestParamDoubleDMA(void*  src0, void*  src1, void* dst0,   void* dst1, int intSize0, int intSize1);
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitMatrixDMA() 
		* \return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива первого источника
		* <p>2 - длина строки матрицы не четное число
		* <p>3 - нечетное растояние между строками у матрицы источника (srcStride32)
		* <p>4 - нечетный адрес массива приемника
		* <p>5 - нечетное растояние между строками у матрицы приемника (dstStride32)
	*/
	// error code descriptoin 
	///// matrix DMA
	// 1 stands for src is odd
	// 2 stands for width is odd
	// 3 stands for srcStride32 is odd
	// 4 stands for dst is odd
	// 5 stands for dstStride32 is odd
	int halTestParamMatrixDMA(void*  src,  int  width,int  height, int srcStride32,  void* dst, int dstStride32);
	//error code description
	///// packet DMA
	//// function returns error code in 4 LSB and from 4th bits number of chaine were erroneous parametr was detected
 	// 1 stands for odd address of src array
 	// 2 stands for odd address of dst array
 	// 4 stands for odd size32 of array 
	/**
		* \brief Функция возвращает код ошибки для параметров функции halInitPacketDMA().
		* <p>Сам код ошибки находится непосредственно в первых 4х битах (LSB), а индекс цепочки хранится в битах начиная с 4 (нумерация битов начинается с 0)   
		* /return
		* Возвращает код ошибки
		* <p>1 - нечетный адрес массива источника
		* <p>2 - нечетный адрес массива приемника
		* <p>2 - нечетное колличество 32 битных слов для копирования
		*
		*
	*/
	int halTestPacketDMA(void** src,  void** dst,  int* size32); 
#ifdef __cplusplus
		};
#endif

