 PASO 1.1: Mover archivos sin modificar
Archivos a mover (copy-paste directo):

excelService.ts → lib/excel/reader.ts
db.ts → lib/db.ts (si no está ya)
security.ts → lib/security/sanitize.ts
UserInfo.tsx → components/shared/UserInfo.tsx (si no está ya)

Acción: Solo mover, NO modificar nada todavía.
Checklist:

 lib/excel/reader.ts existe y tiene el código de lectura de Excel
 lib/db.ts existe con ObtenerPool()
 lib/security/sanitize.ts tiene validarIdsNumericos()
 components/shared/UserInfo.tsx copiado


PASO 1.2: Crear archivos vacíos con estructura básica
Crear estos archivos con solo la estructura, sin lógica:
Archivo: types/common.ts
typescript// Tipos compartidos entre todas las entidades

export interface ResultadoValidacion {
  exito: boolean;
  validacion?: {
    validos: any[];
    invalidos: any[];
    resumen: { total: number; validos: number; invalidos: number };
  };
  mensaje?: string;
}

export interface ResultadoProceso {
  exito: boolean;
  mensaje: string;
  detalles?: any;
}

export type TipoOperacion = 'altas' | 'bajas';
export type Entidad = 'farmacias.mandatarias' | 'prestaciones' | 'prestadores' | 'coberturas';
Archivo: config/constants.ts
typescript// Constantes globales

export const TAMANO_LOTE = 2000;
export const TIMEOUT_DB = 60000;
export const MAX_FILE_SIZE = 10 * 1024 * 1024; // 10MB
Checklist:

 types/common.ts creado con interfaces base
 config/constants.ts creado con constantes


PASO 1.3: Crear config de procesos
Archivo: config/procesos.config.ts
Qué hacer:

Definir metadata de cada entidad (título, descripción, ícono, color, ruta)
Usar esta estructura para que sea fácil iterar en el landing

Pistas:

Cada proceso debe tener: id, titulo, descripcion, icono (nombre de ícono de lucide-react), color, ruta
Exportar como objeto indexado por ID
Ejemplo estructura:

typescriptexport const PROCESOS = {
  'farmacias.mandatarias': {
    id: 'farmacias.mandatarias',
    titulo: 'Farmacias y Mandatarias',
    descripcion: 'Gestión de altas y bajas...',
    icono: 'Pharmacy', // de lucide-react
    color: 'blue',
    ruta: '/farmacias.mandatarias'
  },
  // ... resto
}
Checklist:

 config/procesos.config.ts creado
 Definidas las 4-5 entidades con todos los campos
 Exportado como objeto y como array (para iterar)


📋 FASE 2: CREAR UTILIDADES COMPARTIDAS
PASO 2.1: Implementar utils
Archivo: lib/utils/responses.ts
Qué hacer:

Crear funciones helper para retornar respuestas consistentes
Funciones sugeridas:

successResponse(data, mensaje?)
errorResponse(mensaje, detalles?)
validationResponse(validos, invalidos)



Pistas:

Usar los tipos de types/common.ts
Retornar siempre la misma estructura

Checklist:

 Funciones helper creadas
 Usan tipos de common.ts


Archivo: lib/utils/errors.ts
Qué hacer:

Crear clase de error customizada
Función para sanitizar errores antes de enviar al cliente
Función sanitizeError(error) que retorna mensaje genérico en producción

Pistas:

No retornar stack traces al cliente
Loggear detalles completos internamente
Retornar mensajes user-friendly

Checklist:

 Clase AppError creada
 Función sanitizeError() implementada


Archivo: lib/utils/logger.ts
Qué hacer:

Funciones para loggear operaciones
Por ahora puede ser simple console.log con formato
Estructura sugerida: log(nivel, mensaje, metadata)
Niveles: info, warn, error, debug

Pistas:

Agregar timestamp
Agregar contexto (usuario, entidad, operación)
Preparar para después migrar a Winston o similar

Checklist:

 Funciones logInfo(), logError(), logWarn() creadas
 Formato consistente con timestamp


PASO 2.2: Implementar base validator
Archivo: lib/validators/base.validator.ts
Qué hacer:

Crear funciones de validación reutilizables
Funciones sugeridas:

validarCUIT(cuit: string)
validarEmail(email: string)
validarTelefono(tel: string)
validarNumeroPositivo(num: any)
validarFecha(fecha: any)
validarCampoRequerido(valor: any, nombreCampo: string)
validarLongitud(valor: string, min: number, max: number, nombreCampo: string)



Pistas:

Cada función retorna { valido: boolean, error?: string }
CUIT: validar formato y dígito verificador
Email: regex básico
Teléfono: solo números, longitud razonable

Checklist:

 Al menos 5 validaciones comunes implementadas
 Retornan estructura consistente
 Mensajes de error claros


PASO 2.3: Implementar base service
Archivo: lib/sql/base.service.ts
Qué hacer:

Crear función genérica para ejecutar ALTAS
Crear función genérica para ejecutar BAJAS
Manejar transacciones, lotes, errores

Estructura sugerida:
typescriptexport async function ejecutarAltaGenerica(
  spName: string,
  registros: any[],
  mapearParametros: (registro: any, request: sql.Request) => void
) {
  // Lógica transaccional genérica
  // Iterar registros
  // Llamar mapearParametros para cada uno
  // Manejar errores por lote
  // Retornar resultado
}

export async function ejecutarBajaGenerica(
  tabla: string,
  ids: number[],
  campoId: string = 'id'
) {
  // Lógica para UPDATE baja_fecha = GETDATE()
  // WHERE campoId IN (ids) AND baja_fecha IS NULL
}
Pistas:

Reutilizar lógica de sqlService.ts pero hacerla genérica
El mapearParametros es una función callback que cada servicio específico provee
Manejar transacciones con BEGIN/COMMIT/ROLLBACK
Retornar ResultadoProceso de common.ts

Checklist:

 ejecutarAltaGenerica() implementada
 ejecutarBajaGenerica() implementada
 Manejo de transacciones correcto
 Retorna estructura de common.ts


📋 FASE 3: MIGRAR FARMACIAS (PILOTO)
PASO 3.1: Crear types de farmacias
Archivo: types/farmacias.mandatarias.ts
Qué hacer:

Extraer las interfaces que tenías en sqlService.ts
Crear interfaces para:

Farmacia (datos completos de una farmacia)
Mandataria (datos completos de una mandataria)
RegistroAltaFarmacia (lo que necesita el SP)
RegistroAltaMandataria
RegistroBajaFarmacia
RegistroBajaMandataria



Pistas:

Mirar los campos de las tablas que me pasaste al principio
Usar tipos correctos (number, string, Date | null, etc.)

Checklist:

 Interfaces creadas con todos los campos
 Tipos correctos para cada campo
 Campos opcionales marcados con ? o | null


PASO 3.2: Crear validator de farmacias
Archivo: lib/validators/farmacias.validator.ts
Qué hacer:

Crear función validarFarmacia(farmacia: any, fila: number)
Crear función validarMandataria(mandataria: any, fila: number)
Usar las funciones de base.validator.ts

Estructura sugerida:
typescriptexport function validarFarmacia(registro: any, fila: number) {
  const errores: string[] = [];
  
  // Validar cada campo usando funciones de base.validator
  // Ejemplo:
  const cuitResult = validarCUIT(registro.cuit);
  if (!cuitResult.valido) errores.push(cuitResult.error!);
  
  // ... más validaciones
  
  return {
    valido: errores.length === 0,
    errores
  };
}
Pistas:

Validar TODOS los campos requeridos
Usar validadores de base.validator.ts
Validaciones específicas de negocio (ej: si manda es requerido)

Checklist:

 validarFarmacia() implementada
 validarMandataria() implementada
 Usan funciones de base.validator.ts
 Retornan array de errores


PASO 3.3: Crear service de farmacias
Archivo: lib/sql/farmacias.services.ts
Qué hacer:

Migrar las funciones de sqlService.ts relacionadas a farmacias
Refactorizar para usar ejecutarAltaGenerica() y ejecutarBajaGenerica()

Funciones a crear:

ejecutarAltasFarmacias(registros) - usa ejecutarAltaGenerica()
ejecutarBajasFarmacias(ids) - usa ejecutarBajaGenerica()
ejecutarAltasMandatarias(registros)
ejecutarBajasMandatarias(registros)

Pistas:

Para altas, crear función mapearParametrosFarmacia(registro, request) que hace todos los request.input()
Pasarla como callback a ejecutarAltaGenerica()
Para bajas, solo llamar ejecutarBajaGenerica('farmacias', ids, 'farma')

Checklist:

 4 funciones implementadas (altas/bajas farmacias y mandatarias)
 Usan funciones de base.service.ts
 Importan tipos de types/farmacias.mandatarias.ts


PASO 3.4: Crear actions de farmacias
Archivo: actions/farmacias.mandatarias.ts
Qué hacer:

Migrar lógica de las 3 API routes (validar, altas, bajas)
Crear 3 server actions:

validarFarmacias(formData, tipo: 'farmacias' | 'mandatarias')
procesarAltasFarmacias(registros)
procesarBajasFarmacias(ids)
(y lo mismo para mandatarias)



Estructura:
typescript'use server';

import { leerExcel } from './shared';
import { validarFarmacia } from '@/lib/validators/farmacias.validator';
import { ejecutarAltasFarmacias } from '@/lib/sql/farmacias.services';

export async function validarFarmacias(formData: FormData, tipo: 'altas' | 'bajas') {
  // 1. Leer Excel usando función compartida
  // 2. Validar cada registro
  // 3. Separar válidos e inválidos
  // 4. Retornar resultado
}

export async function procesarAltasFarmacias(registros: any[]) {
  // 1. Llamar servicio SQL
  // 2. Loggear operación
  // 3. Retornar resultado
}
Pistas:

Agregar 'use server' al principio del archivo
Manejar errores con try/catch
Usar logInfo() y logError() de utils
Retornar tipos de common.ts

Checklist:

 Actions creadas con 'use server'
 Llaman a validators y services correspondientes
 Loggean operaciones
 Retornan tipos consistentes


PASO 3.5: Crear action shared
Archivo: actions/shared.ts
Qué hacer:

Función leerExcel(formData) - wrapper de lib/excel/reader.ts
Función verificarSesion() - verificar cookie kortex_session

Estructura:
typescript'use server';

import { cookies } from 'next/headers';
import { leerArchivoExcel } from '@/lib/excel/reader';

export async function leerExcel(formData: FormData) {
  const file = formData.get('file') as File;
  // Validar file
  // Convertir a buffer
  // Llamar leerArchivoExcel
  // Retornar resultado
}

export async function verificarSesion() {
  const cookieStore = await cookies();
  const session = cookieStore.get('kortex_session');
  // Validar sesión
  // Retornar usuario o null
}
Checklist:

 leerExcel() implementada
 verificarSesion() implementada
 Manejan errores correctamente


📋 FASE 4: ACTUALIZAR UI
PASO 4.1: Extraer ValidationResults
Archivo: components/shared/ValidationResults.tsx
Qué hacer:

Extraer la sección de resultados del FileUpLoader.tsx actual
Crear componente independiente que recibe props:

validacion: ResultadoValidacion | null



Pistas:

Mostrar resumen (válidos/inválidos)
Lista de errores expandible
Diseño glass-card como el actual

Checklist:

 Componente creado
 Recibe validacion como prop
 Renderiza igual que antes


PASO 4.2: Extraer ProgressBar
Archivo: components/shared/ProgressBar.tsx
Qué hacer:

Extraer la barra de progreso del FileUpLoader.tsx
Props:

progreso: { actual: number, total: number, porcentaje: number }
mensajeEstado: string
visible: boolean



Checklist:

 Componente creado
 Recibe props necesarias
 Animaciones funcionan


PASO 4.3: Refactorizar FileUpLoader
Archivo: components/shared/FileUpLoader.tsx
Qué hacer:

Hacerlo genérico con props:

entidad: Entidad
tipo: TipoOperacion
tipoEntidad: 'farmacias' | 'mandatarias' (para farmacias.mandatarias)


Cambiar fetch() por llamadas a actions
Usar ValidationResults y ProgressBar como componentes

Estructura:
typescriptinterface Props {
  entidad: Entidad;
  tipo: TipoOperacion;
  tipoEntidad?: string; // para farmacias.mandatarias
}

export default function FileUpLoader({ entidad, tipo, tipoEntidad }: Props) {
  // Construir nombres dinámicamente
  // Importar action correspondiente
  // Renderizar ValidationResults y ProgressBar
}
Pistas:

Usar imports dinámicos o switch para llamar la action correcta
O mejor: recibir las actions como props también

Checklist:

 Props genéricas agregadas
 Usa actions en vez de fetch
 Usa ValidationResults y ProgressBar
 Funciona con cualquier entidad


PASO 4.4: Crear ProcessSelector
Archivo: components/shared/ProcessSelector.tsx
Qué hacer:

Card clickeable para el landing
Props:

proceso (objeto de procesos.config.ts)



Estructura:

Card con glass-card style
Ícono grande (de lucide-react)
Título y descripción
Click → navega a la ruta

Checklist:

 Componente creado
 Recibe proceso como prop
 Renderiza ícono dinámicamente
 Link funciona


PASO 4.5: Crear Landing
Archivo: app/page.tsx
Qué hacer:

Grid de ProcessSelector
Iterar sobre PROCESOS de config
Diseño responsive

Pistas:

Usar grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3
Incluir UserInfo
Título principal

Checklist:

 Itera sobre procesos.config
 Renderiza ProcessSelector para cada uno
 Diseño responsive
 UserInfo visible


PASO 4.6: Crear page de farmacias
Archivo: app/farmacias.mandatarias/page.tsx
Qué hacer:

Componente con tabs "Farmacias" y "Mandatarias"
Cada uno con subtabs "Altas" y "Bajas"
Renderiza FileUpLoader con props correspondientes

Estructura:

Tabs principales: Farmacias | Mandatarias
Subtabs: Altas | Bajas
Cada combinación renderiza:

tsx  <FileUpLoader 
    entidad="farmacias.mandatarias" 
    tipo="altas"
    tipoEntidad="farmacias"
  />
Pistas:

Usar estado para controlar tabs activos
Diseño glass-card

Checklist:

 Tabs implementados
 FileUpLoader se renderiza correctamente
 Cambiar tabs funciona


📋 FASE 5: SEGURIDAD
PASO 5.1: Implementar auth
Archivo: lib/security/auth.ts
Qué hacer:

Función verificarAutenticacion() que verifica sesión
Wrapper para actions: withAuth(action)

Estructura:
typescriptexport async function verificarAutenticacion() {
  const session = await verificarSesion(); // de actions/shared
  if (!session) {
    throw new Error('No autenticado');
  }
  return session;
}

export function withAuth(action: Function) {
  return async (...args: any[]) => {
    await verificarAutenticacion();
    return action(...args);
  };
}
Checklist:

 verificarAutenticacion() implementada
 withAuth() wrapper creado
 Se usa en todas las actions


PASO 5.2: Implementar rate limiting
Archivo: lib/security/ratelimit.ts
Qué hacer:

Implementar rate limiting básico
Puede ser in-memory simple por ahora
Estructura: Map con usuario → { count, lastReset }

Pistas:

Límite: 10 requests por minuto por usuario
Limpiar contador cada minuto

Checklist:

 Rate limiter implementado
 Se usa en actions


📋 FASE 6: REPLICAR PARA OTRAS ENTIDADES
PASO 6.1-6.4: Repetir para cada entidad
Para CADA una de las otras entidades (prestaciones, prestadores, coberturas):

Crear types/{entidad}.ts
Crear lib/validators/{entidad}.validator.ts
Crear lib/sql/{entidad}.services.ts
Crear actions/{entidad}.ts
Crear app/{entidad}/page.tsx

Checklist por entidad:

 Prestaciones completa
 Prestadores completa
 Coberturas completa