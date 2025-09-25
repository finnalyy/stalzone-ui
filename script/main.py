import pymem
import pymem.process
import sys

# Опкод для инструкции `shl rcx,04`
pattern = b'\xA9\xFF\xF7\xFF\xFF'

# Название процесса
process_name = "stalcraft.exe"

try:
    # Открываем процесс
    pm = pymem.Pymem(process_name)
except pymem.exception.ProcessNotFound:
    print(f"Процесс {process_name} не найден.")
    sys.exit(1)
except Exception as e:
    print(f"Ошибка при открытии процесса: {e}")
    sys.exit(1)

try:
    # Получаем модуль nvoglv64.dll
    module_info = pymem.process.module_from_name(pm.process_handle, "nvoglv64.dll")
except pymem.exception.ModuleNotFoundError:
    print("Модуль nvoglv64.dll не найден в процессе.")
    sys.exit(1)
except Exception as e:
    print(f"Ошибка при получении информации о модуле: {e}")
    sys.exit(1)

# Базовый адрес и размер модуля
module = module_info.lpBaseOfDll
module_size = module_info.SizeOfImage

# Проходим по памяти модуля и ищем все паттерны
def find_all_patterns(handle, base, size, pattern):
    addresses = []
    chunk_size = 4096  # Читаем блоки памяти по 4KB

    for i in range(0, size, chunk_size):
        try:
            # Читаем блок памяти
            buffer = pm.read_bytes(base + i, min(chunk_size, size - i))
        except pymem.exception.MemoryReadError:
            print(f"Ошибка чтения памяти в блоке: 0x{base + i:X}")
            continue

        offset = 0
        while offset != -1:
            offset = buffer.find(pattern, offset)
            if offset != -1:
                addresses.append(base + i + offset)
                offset += len(pattern)  # Сдвигаем указатель для поиска следующих вхождений
    return addresses

# Ищем все совпадения в модуле
try:
    results = find_all_patterns(pm.process_handle, module, module_size, pattern)
except Exception as e:
    print(f"Ошибка при поиске паттернов: {e}")
    sys.exit(1)

if results:
    for index, address in enumerate(results):
        if index % 2 == 0:
            if index == 0:
                print(f'chams -> 0x{address:X}')
            if index == 8:
                print(f'wallhack -> 0x{address:X}')
            print(f"0x{address:X}")
            if index == 14:
                print(f'fuck models -> 0x{address:X}')
else:
    print("Инструкции не найдены.")
