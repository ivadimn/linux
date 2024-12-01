// vim: colorcolumn=80 ts=4 sw=4

/* Это чрезвычайно простой аннотированный файл .c, который реализует пользовательский
виджет в gtk4 с gobject. Все, что он делает, это подклассифицирует GtkWidget и создает
виджет, который является просто кнопкой.

Требуется: glib >= 2.44, gtk4 >= 4.0.0

Это шаблон для FINAL (в отличие от DERIVABLE типа gobject.

Финальные типы проще. Вы делаете большую часть работы в файле C и просто
открываете несколько открытых методов в файле .h.
Другими словами, поскольку его нельзя подклассифицировать дальше, вы вообще не
открываете открыто внутренности объекта, и он просто определяется и обрабатывается

с использованием стандартных функций GObject/GTK.
 */
#include <stdio.h>
#include "demowidget.h"

/* INTERNAL DECLARATIONS */

/* Это не является строго необходимым, поскольку мы сохранили все в естественном рабочем
порядке ниже. Но G_DEFINE_TYPE *не* предоставляет объявления для этих
функций деструктора, поэтому если вы измените порядок конструкторов и
деструкторов, определенных ниже, вы получите ошибку.
 */

static void demo_widget_dispose (GObject *object);
static void demo_widget_finalize (GObject *object);

/* GOBJECT DEFINITION */

/* Сначала создайте основную структуру, содержащую внутренности объекта.
Следуйте формату подчеркивания, за которым следует имя типа CamelCase, 
которое вы создали в заголовочном файле. Typedef автоматически генерируется
макросами и, как ожидается, будет определен здесь.
 */

struct _DemoWidget
{
	/* тип подкласса - убедитесь, что вы *не* используете указатель. Оставьте это как
		первый член.
	 */
	GtkWidget parent_instance;

	/* Здесь размещаются пользовательские мясо и картофель. Сейчас мы просто создаем
		кнопку. Здесь вы *используете* указатели, когда хотите включить другие
		объекты в структуру.
	*/
	GtkWidget *button;
};

/* как только основная структура определена, как указано выше, вот следующий макрос. Убедитесь, что
это идет до того, как делать что-либо еще. Порядок имеет значение.

Первые два аргумента макроса являются шаблонными. Последний — стандартный
заглавный макрос TYPE *того, что вы подклассифицируете.* Самый низкий, 
который вы можете использовать, — это
G_TYPE_OBJECT для сырого gobject. В этом случае мы подклассифицируем
GtkWidget, поэтому этот макрос типа указан ниже.
*/
G_DEFINE_TYPE (DemoWidget, demo_widget, GTK_TYPE_WIDGET)


/* METHOD DEFINITIONS */

/* эта функция используется для построения виджета. Обратите внимание, 
что хотя в документации упоминается
что-то как функция 'instance_init', подпрограмма на самом деле
называется *_init, а не *_instance_init. Это меня сначала смутило.
 */
static void
demo_widget_init (DemoWidget *self)
{
	/* Not necessary, but it is a useful shorthand. */
	printf("demo_widget_init");
	GtkWidget *widget = GTK_WIDGET(self);

	self->button = gtk_button_new_with_label ("Hello, world!");
	gtk_widget_set_hexpand (self->button, TRUE);
	gtk_widget_set_parent (self->button, widget);
}

/* Это один из этапов процесса деструктора вместе с _finalize.
Здесь вы в основном уничтожаете элементы, которые подсчитываются ссылками (например, GObject-y
stuff), а не нет (например, строка или указатель FILE *, который должен
быть освобожден / закрыт, а не быть отброшенным).

Важно помнить, что этот метод может быть вызван несколько раз. 
Не учитывать это — ошибка программиста. Если вы отмените ссылку на GObject,
уже имеющий счетчик ссылок 0, вы получите ошибку времени выполнения. Ниже
о том, как мы это учитываем.

См. Часть 1B этой серии о проблемах, с которыми вы можете столкнуться, 
если не будете с этим работать правильно.

См. также:
  https://developer-old.gnome.org/gobject/stable/howto-gobject-destruction.html
 */
static void
demo_widget_dispose (GObject *object)
{
	printf("demo_widget_dispose");
	DemoWidget *self = DEMO_WIDGET(object);

	/* g_clear_pointer — очень полезная функция. Она вызывает функцию по вашему выбору, 
	чтобы отменить ссылку или освободить указатель, а затем устанавливает этот указатель в NULL.
	Если она вызывается для указателя NULL, она просто ничего не делает. Это позволяет
	идеально решать проблему многократного вызова _dispose.

	Здесь gtk_widget_unparent — это стандартная функция GTK для отделения
	дочернего виджета от его родителя, которая отменяет ссылку на этот виджет за кулисами.

	Обратите внимание, что g_clear_pointer принимает указатель *на* gpointer, а не
	прямой gpointer. Это необходимо для того, чтобы функция обнулила
	указатель.
	 */
	g_clear_pointer (&self->button, gtk_widget_unparent);

	/* Последний шаг: цепочка (шаблон)
	Это называется «цепочкой» в документации gobject. *_parent_class
	автоматически генерируется макросами - согласно gtype.h, это статическая
	переменная, указывающая на родительский класс.
	*/
	G_OBJECT_CLASS(demo_widget_parent_class)->dispose (object);
}

/* Второй и последний этап процесса уничтожения. 
См. _finalize выше для получения дополнительной информации. 
Пока нам нечего сюда вставить, но мы включаем это для
потомков и для легкого добавления позже; поэтому мы просто вручную соединяем на данный момент.
 */
static void
demo_widget_finalize (GObject *object)
{
	/* --- */
	printf("demo_widget_finalize");
	/* Всегда связывайтесь с родительским классом; 
	как и в случае с dispose(), finalize() генерируется автоматически и, 
	таким образом, всегда гарантированно существует 
	в таблице виртуальных функций родительского класса
	*/
	G_OBJECT_CLASS(demo_widget_parent_class)->finalize(object);
}

/* этот метод запускается в первый раз, когда класс *когда-либо* используется, но не
снова.

См.:
https://developer-old.gnome.org/gobject/stable/howto-gobject-destruction.html
для получения дополнительной информации о том, что вам нужно поместить в эту подпрограмму 
относительно деструкторов.

Примечание: я все время путался в том, что означает 'klass' и почему он пишется
именно так. Это просто для того, чтобы избежать использования токена 'class' 
сам по себе, который является
зарезервированным словом c++. Вы можете назвать его как-то иначе, 
например 'gclass', если хотите, но он идиоматически назывался 'klass' более 20 лет, так что,
вероятно, лучше просто придерживаться этого.
 */
static void
demo_widget_class_init (DemoWidgetClass *klass)
{
	printf("demo_widget_class_init");
	/* Создайте стенографию, чтобы избежать некоторых приведений. */
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	/* Сопоставьте «виртуальные функции» dispose и finalize 
	с функциями, которые мы определили выше.
	 */
	object_class->dispose = demo_widget_dispose;
	object_class->finalize = demo_widget_finalize;

	/* Сообщите нашему виджету, какой тип «менеджера компоновки» использовать, 
	чтобы он знал, как расположить виджеты. 
	Тип Box — хороший вариант по умолчанию.
	*/
	gtk_widget_class_set_layout_manager_type (GTK_WIDGET_CLASS(klass),
			GTK_TYPE_BOX_LAYOUT);
}

/* и наконец, вот фактическое определение нашей публичной функции для создания
экземпляра нашего объекта.

Он принимает GType типа GObject, за которым (необязательно) следуют пары имя/значение,
чтобы задать «свойства» объекта, за которыми следует NULL, когда вы закончите. Поскольку
у нас нет никаких свойств, которые мы собираемся инстанцировать, мы просто передаем NULL
после GType. Мы обсудим свойства GObject позже.
 */
GtkWidget *
demo_widget_new (void)
{
	printf("demo_widget_new");
	return g_object_new (DEMO_TYPE_WIDGET, NULL);
}
