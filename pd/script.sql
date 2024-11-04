-- DROP TABLE public.teacher;
CREATE DATABASE lna;
CREATE USER develop WITH ENCRYPTED PASSWORD '7922448';
GRANT ALL PRIVILEGES ON DATABASE lna TO develop;
ALTER DATABASE lna OWNER TO develop;
REVOKE CONNECT ON DATABASE lna FROM PUBLIC;
# --------------------------------------------------------------------------
CREATE USER admin WITH ENCRYPTED PASSWORD '7922448';
GRANT ALL PRIVILEGES ON DATABASE lna TO admin;
CREATE USER student WITH ENCRYPTED PASSWORD '7922448';
GRANT ALL PRIVILEGES ON DATABASE lna TO student;
CREATE USER teacher WITH ENCRYPTED PASSWORD '7922448';
GRANT ALL PRIVILEGES ON DATABASE lna TO teacher;


CREATE TABLE public.teacher (
	id serial4 NOT NULL,
	f_fio varchar NOT NULL,
	f_phone varchar NULL,
	f_email varchar NULL,
	f_comment text NULL,
	CONSTRAINT teacher_pk PRIMARY KEY (id)
);

CREATE TABLE public.task (
	id serial4 NOT NULL,
	f_name varchar NULL,
	f_content text NOT NULL,
	f_created timestamp NULL DEFAULT CURRENT_TIMESTAMP,
	teacher_id int4 NULL,
	CONSTRAINT task_pk PRIMARY KEY (id)
);
-- public.task foreign keys
ALTER TABLE public.task ADD CONSTRAINT task_fk FOREIGN KEY (teacher_id) REFERENCES public.teacher(id);

-- DROP TABLE public.variant;

CREATE TABLE public.variant (
	id serial4 NOT NULL,
	f_name varchar NULL,
	f_created timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
	teacher_id int4 NOT NULL,
	CONSTRAINT variant_pk PRIMARY KEY (id)
);
-- public.variant foreign keys
ALTER TABLE public.variant ADD CONSTRAINT variant_fk FOREIGN KEY (teacher_id) REFERENCES public.teacher(id);


-- DROP TABLE public.task_variant;

CREATE TABLE public.task_variant (
	id serial4 NOT NULL,
	variant_id int4 NOT NULL,
	task_id int4 NOT NULL,
	f_ordinal int4 NULL,
	CONSTRAINT task_variant_pk PRIMARY KEY (id),
	CONSTRAINT task_variant_un UNIQUE (variant_id, task_id)
);
-- public.task_variant foreign keys
ALTER TABLE public.task_variant ADD CONSTRAINT task_variant_fk FOREIGN KEY (task_id) REFERENCES public.task(id);
ALTER TABLE public.task_variant ADD CONSTRAINT task_variant_fk_1 FOREIGN KEY (variant_id) REFERENCES public.variant(id);

CREATE TABLE public.student (
	id serial4 NOT NULL,
	f_fio varchar NOT NULL,
	f_email varchar NULL,
	f_comment text NULL,
	CONSTRAINT student_pk PRIMARY KEY (id)
);

CREATE TABLE public.stgroup (
	id serial4 NOT NULL,
	f_title varchar NOT NULL,
	f_comment varchar NULL,
	CONSTRAINT groups_pk PRIMARY KEY (id)
);

CREATE TABLE public.appuser (
    id serial4 NOT NULL,
    f_login varchar NOT NULL,
    f_password_hash varchar,
    f_enabled bool NOT NULL DEFAULT TRUE,
    f_created timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    f_expire timestamp,
    CONSTRAINT appuser_pk PRIMARY KEY (id),
	CONSTRAINT appuser_un UNIQUE (f_login)
);

CREATE TYPE user_role AS ENUM ('teacher', 'student', 'admin');
ALTER TABLE public.appuser ADD f_role user_role NULL;
ALTER TABLE public.appuser ADD f_salt varchar NULL;


ALTER TABLE public.teacher ADD user_id int NULL;
ALTER TABLE public.teacher ADD CONSTRAINT teacher_fk FOREIGN KEY (user_id) REFERENCES public.appuser(id);

ALTER TABLE public.student ADD user_id int NULL;
ALTER TABLE public.student ADD CONSTRAINT student_fk FOREIGN KEY (user_id) REFERENCES public.appuser(id);

ALTER TABLE public.student ALTER COLUMN user_id SET NOT NULL;
ALTER TABLE public.teacher ALTER COLUMN user_id SET NOT NULL;

ALTER TABLE public.appuser ALTER COLUMN f_role SET NOT NULL;
ALTER TABLE public.appuser ALTER COLUMN f_salt SET NOT NULL;


