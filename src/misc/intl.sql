/***
 *
 *  Usage example
 *  -------------
 *
 *  input 'intl.sql';
 *
 *  execute procedure sp_register_character_set ('CHARSET_NAME', 1);
 *  execute procedure sp_register_collation ('CHARSET_NAME', 'COLLATION_NAME');
 *  commit;
 *
 *  execute procedure sp_unregister_collation ('CHARSET_NAME', 'COLLATION_NAME');
 *  execute procedure sp_unregister_character_set ('CHARSET_NAME');
 *  commit;
 *
 */


set term !;


create or alter procedure sp_register_character_set
(
	name char(31) character set unicode_fss,
	max_bytes_per_character smallint
)
as
	declare variable id smallint;
	declare variable temp_id smallint;
begin
	name = upper(name);
	id = 255;

	for select rdb$character_set_id
			from rdb$character_sets
			order by rdb$character_set_id desc
		into :temp_id do
	begin
		if (temp_id = id) then
			id = id - 1;
		else
			break;
	end

	insert into rdb$character_sets
		(rdb$character_set_name, rdb$character_set_id, rdb$system_flag, rdb$bytes_per_character)
		values (:name, :id, 0, :max_bytes_per_character);

	insert into rdb$collations
		(rdb$collation_name, rdb$collation_id, rdb$character_set_id, rdb$system_flag)
		values (:name, 0, :id, 0);
end!


create or alter procedure sp_unregister_character_set
(
	name char(31) character set unicode_fss
)
as
	declare variable id smallint;
begin
	name = upper(name);

	select rdb$character_set_id
		from rdb$character_sets
		where rdb$character_set_name = :name
		into :id;

	delete from rdb$collations
		where rdb$character_set_id = :id;

	delete from rdb$character_sets
		where rdb$character_set_id = :id;
end!


create or alter procedure sp_register_collation
(
	character_set char(31) character set unicode_fss,
	name char(31) character set unicode_fss,
	base_name char(31) character set unicode_fss = null,
	attributes smallint = null,
	specific_attributes blob sub_type text character set unicode_fss = null
)
as
	declare variable id smallint;
	declare variable temp_id smallint;
	declare variable charset_id smallint;
begin
	character_set = upper(character_set);
	name = upper(name);
	base_name = coalesce(upper(base_name), name);
	id = 126;

	select rdb$character_set_id
		from rdb$character_sets
		where rdb$character_set_name = :character_set
		into :charset_id;

	for select rdb$collation_id
			from rdb$collations
			where rdb$character_set_id = :charset_id
			order by rdb$collation_id desc
		into :temp_id do
	begin
		if (temp_id = id) then
			id = id - 1;
		else
			break;
	end

	insert into rdb$collations
		(rdb$collation_name, rdb$collation_id, rdb$character_set_id, rdb$system_flag,
		 rdb$base_collation_name, rdb$collation_attributes, rdb$specific_attributes)
		values (:name, :id, :charset_id, 0, :base_name, :attributes, :specific_attributes);
end!


create or alter procedure sp_unregister_collation
(
	character_set char(31) character set unicode_fss,
	name char(31) character set unicode_fss
)
as
	declare variable id smallint;
begin
	character_set = upper(character_set);
	name = upper(name);

	select rdb$character_set_id
		from rdb$character_sets
		where rdb$character_set_name = :character_set
		into :id;

	delete from rdb$collations
		where rdb$character_set_id = :id and rdb$collation_name = :name;
end!


set term ;!
commit;
