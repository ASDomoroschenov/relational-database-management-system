#ifndef BUILDER_CONCRETE_H
#define BUILDER_CONCRETE_H

#include "../builder/builder.h"
#include "../logger_concrete/logger_concrete.h"
#include <iostream>

class builder_concrete final : public builder
{

private:

	std::vector<std::pair<std::string, logger::severity> > _streams;

public:

	builder* add_stream(
		std::string const& name_stream,
		logger::severity level) override;

	builder* clear() override;

	logger* build() const override;

};

#endif