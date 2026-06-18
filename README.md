# Ejercicio 2: Gastos Personales

## Respuestas de Diseño: ¿Qué patrón desacopla qué parte del diseño?
* **Factory Method:** Desacopla la lógica de *creación* de los formatos de salida (CSV, JSON, Texto). El cliente no necesita saber cómo se instancian ni usar `if/else` repetidos.
* **Decorator:** Desacopla el *procesamiento adicional* (como sumar los totales o agregar un registro de auditoría) del formato base. Permite agregar estas funcionalidades de forma dinámica sin modificar las clases de exportación originales.
* **Strategy:** Desacopla el *algoritmo de ordenamiento* de la estructura de datos. Permite cambiar el criterio de ordenación (por monto, por fecha/categoría) pasándolo como parámetro sin alterar la función de ordenamiento principal ni los datos.
* <img width="1878" height="820" alt="image" src="https://github.com/user-attachments/assets/da993d14-e290-40f2-a3bd-de84bcfca0ec" />
